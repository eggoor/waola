#include "pch.h"

#include "Resource.h"
#include "WaolawTaskData.h"
#include "WndMain.h"
#include "ListView.h"
#include "PermanentStorage.h"
#include "CommandHandlers.h"
#include "StatusMonitor.h"

#include "WaolaW.h"

extern HWND g_hwndMain;

static HANDLE DoStartTask(WaolawTaskData*);
static int CompareHostView(const void*, const void*);

static waolasc_t* g_WaolaCo;
static HDPA g_StoredHosts;
static HDPA g_hostCollection;
static HANDLE g_hThread;
static CRITICAL_SECTION g_cs;
static BOOL g_bBatchDelete;
static int g_nTaskId;
static waolasc_subscribe_data_t g_StateCbi;
static waolava_subscribe_data_t g_VaultCbi;

BOOL WaolawCreate()
{
	if (!InitializeCriticalSectionAndSpinCount(&g_cs, 0x0400)) {
		wlog_if_level(wll_error, "InitializeCriticalSectionAndSpinCount failed\n");
		HandleError(TRUE);
		goto end;
	}

	memset(&g_StateCbi, 0, sizeof(g_StateCbi));
	g_StateCbi.cb = &WaolawOnStateChanged;
	memset(&g_VaultCbi, 0, sizeof(g_VaultCbi));
	g_VaultCbi.cb = &WaolawOnHostDiscovered;

	g_WaolaCo = wmake_waolasc_def(&g_StateCbi, &g_VaultCbi);

	if (!g_WaolaCo) {
		wlog_if_level(wll_warning, "Unable to make waola scanner\n");
		goto end;
	}

	g_hostCollection = DPA_Create(4);

	if (!g_hostCollection) {
		wlog_if_level(wll_warning, "Unable to make hosts dynamic pointer array\n");
	}
	
end:
	return !!g_WaolaCo && !!g_hostCollection;
}

void WaolawDestroy()
{
	WaolawCancelTask();
	WaolaWaitForTaskFinish();
	waolasc_unsubscribe_vault(g_WaolaCo, &g_VaultCbi);
	waolasc_unsubscribe_state(g_WaolaCo, &g_StateCbi);
	SaveHosts(g_hostCollection);

	waolasc_free(g_WaolaCo);

	for (int i = DPA_GetPtrCount(g_hostCollection); i --> 0;) {
		HostView* hostView = DPA_FastGetPtr(g_hostCollection, i);
		HostViewDestroy(hostView);
	}

	DPA_Destroy(g_hostCollection);

	DeleteCriticalSection(&g_cs);
}

void WaolawOnTaskFinished(HWND hWndMain)
{
	CloseHandle(g_hThread);
	g_hThread = NULL;
}

wtask_t WaolawGetCurrentTasks()
{
	return waolasc_get_current_tasks(g_WaolaCo);
}

void WaolaWakeUp(HostView** selectedHosts, size_t hostCount)
{
	whost_view_t** hosts = walloc(hostCount * sizeof(whost_view_t*));

	for (size_t i = hostCount; i --> 0;) {
		hosts[i] = HostViewGetHostView(selectedHosts[i]);
	}

	wlog_if_level(wll_info, "Waking up %d host(s)\n", hostCount);

	waolah(hosts, hostCount);

	wfree(hosts);
}

void WaolawStartTask(int nTaskId, HWND hWndMain)
{
	WaolawTaskData* td = walloc(sizeof(struct TWaolawTaskData));

	td->nTaskId = nTaskId;
	td->hwndMain = hWndMain;

	g_nTaskId = nTaskId;
	g_hThread = DoStartTask(td);
}

int WaolawGetTaskId()
{
	return g_nTaskId;
}

void WaolawCancelTask()
{
	waolasc_cancel_task(g_WaolaCo);
}

BOOL WaolaWaitForTaskFinish()
{
	DWORD waitResult = WAIT_OBJECT_0;

	if (g_hThread) {
		waitResult = WaitForSingleObject(g_hThread, 5000);
		CloseHandle(g_hThread);
		g_hThread = NULL;
	}

	return waitResult == WAIT_OBJECT_0;
}

void WaolawAddHostToModel(const whost_data_t* hd)
{
	waolasc_add(g_WaolaCo, hd);
}

void WaolawUpdateHost(const HostView* hostView)
{
	whost_view_t* hw = HostViewGetHostView(hostView);
	waolasc_refresh_host(g_WaolaCo, hw);
}

HostView* WaolawGetHost(int x)
{
	return DPA_GetPtr(g_hostCollection, x);
}

void WaolawDeleteHosts(HostView** hostDataList, uint32_t nCount)
{
	g_bBatchDelete = TRUE;

	qsort(hostDataList, nCount, sizeof(HostView*), CompareHostView);

	for (int i = nCount; i --> 0;) {
		WaolawDeleteHost(hostDataList[i]);
	}

	g_bBatchDelete = FALSE;

	RenumberHosts();
}

void WaolawDeleteHost(HostView* hostView)
{
	int x = HostViewGetIndex(hostView);
	whost_view_t* hw = HostViewGetHostView(hostView);
	waolasc_delete_host(g_WaolaCo, hw);
	DPA_DeletePtr(g_hostCollection, x);

	if (!g_bBatchDelete) {
		RenumberHosts();
	}
}

BOOL WaolawIsEmpty()
{
	return waolasc_is_empty(g_WaolaCo);
}

BOOL WaolawLoad()
{
	g_StoredHosts = LoadHosts();
	return DPA_GetPtrCount(g_StoredHosts) > 0;
}

void WaolaProcessStateChanged(wtask_t tasks)
{
	DoUpdateUI(tasks);
}

void WaolaProcessHostDiscovered(waolava_op_t op, whost_view_t* hw)
{
	EnterCriticalSection(&g_cs);

	switch (op)
	{
	case wva_added:
		AddHostToView(hw);
		break;
	case wva_modified:
	{
		const int* pX = whost_view_get_extra_data(hw);

		if (pX) {
			HostView* hostView = DPA_FastGetPtr(g_hostCollection, *pX);
			ListViewUpdateHost(hostView);
		}
		break;
	}
	}

	LeaveCriticalSection(&g_cs);
}

static void WaolawOnStateChanged(const waolasc_cbi_t* const cbi)
{
	if (cbi->tasks > 0) {
		StatusStartMonitoring(g_WaolaCo);
	}
	else {
		StatusStopMonitoring();
	}

	PostMessage(g_hwndMain, WM_WAOLA_STATE_CHANGED,
		(WPARAM)cbi->tasks, 0);
}

static void WaolawOnHostDiscovered(const waolava_cbi_t* const cbi)
{
	PostMessage(g_hwndMain, WM_WAOLA_HOST_DISCOVERED,
		(WPARAM)cbi->opCode, (LPARAM)cbi->hostView);
}

static void AddHostToView(whost_view_t* hw)
{
	int x = DPA_GetPtrCount(g_hostCollection);
	HostView* hostView = HostViewCreate(x, hw);
	if (hostView) {
		DPA_AppendPtr(g_hostCollection, hostView);
		ListViewAddHost(hostView);
	}
	else {
		wlog_if_level(wll_warning, "HostViewCreate failed\n");
	}
}

static void RenumberHosts()
{
	for (int i = DPA_GetPtrCount(g_hostCollection); i --> 0;) {
		HostView* hostView = DPA_FastGetPtr(g_hostCollection, i);
		HostViewSetIndex(hostView, i);
	}
}

static DWORD CALLBACK WaolaTaskProc(LPVOID lpParam)
{
	int rc = 0;
	WaolawTaskData* td = (WaolawTaskData*)lpParam;

	switch (td->nTaskId)
	{
	case ID_WAOLA_RESCAN:
		rc = waolasc_discover(g_WaolaCo);
		break;
	case ID_WAOLA_REFRESH:
		rc = waolasc_refresh(g_WaolaCo);
		break;
	case ID_WAOLA_ADD:
		if (g_StoredHosts) {
			const int n = DPA_GetPtrCount(g_StoredHosts);
			whost_data_t** hd = walloc(n * sizeof(whost_data_t*));
			for (int i = n; i --> 0;) {
				hd[i] = DPA_FastGetPtr(g_StoredHosts, i);
			}
			waolasc_batch_add(g_WaolaCo, hd, n);
			wfree(hd);
			DPA_Destroy(g_StoredHosts);
		}
		break;
	default:
		wlog_if_level(wll_warning, "Unknown task identifier %d\n", td->nTaskId);
	}

	wfree(td);
	PostMessage(td->hwndMain, WM_WAOLA_TASK_FINISHED, rc, 0);

	return 0;
}

static HANDLE DoStartTask(WaolawTaskData* taskData)
{
	HANDLE hThread = CreateThread(
		NULL,						// default security attributes
		0,							// use default stack size	
		WaolaTaskProc,				// thread function name
		taskData,					// argument to thread function 
		0,							// use default creation flags 
		NULL);						// returns the thread identifier

	return hThread;
}

static int CompareHostView(const void* l, const void* const restrict r)
{
	const HostView* hostLiewL = *(HostView**)l;
	const HostView* hostLiewR = *(HostView**)r;
	return HostViewGetIndex(hostLiewL)
		- HostViewGetIndex(hostLiewR);
}
