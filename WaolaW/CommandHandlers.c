#include "pch.h"

#include "Resource.h"
#include "WaolaW.h"
#include "DlgAbout.h"
#include "DlgHostEdit.h"
#include "ToolBar.h"
#include "ListView.h"
#include "Clipboard.h"

#include "CommandHandlers.h"

extern HINSTANCE g_hInst;

void OnCommandWaolaTask(int nTaskId, HWND hWndMain)
{
	SetCursor(LoadCursor(0, IDC_APPSTARTING));

	wtask_t tasks = WaolawGetCurrentTasks();

	if (wt_idle != tasks) {
		BOOL bStopTask = (ID_WAOLA_RESCAN == nTaskId && (wt_discover & tasks))
			|| (ID_WAOLA_REFRESH == nTaskId && (wt_refresh & tasks));
		if (bStopTask) {
			WaolawCancelTask();
		}
	}
	else {
		WaolawStartTask(nTaskId, hWndMain);
	}
}

void OnCommandWakeUp()
{
	OnCommandWakeUpSelected();
}

void OnCommandWakeUpSelected()
{
	uint32_t hostCount;
	HostView** selectedHosts = ListViewGetSelectedHosts(&hostCount);

	if (hostCount > 0) {

		WaolaWakeUp(selectedHosts, hostCount);

		for (int i = hostCount; i --> 0;) {
			ListViewUpdateHost(selectedHosts[i]);
		}

		wfree(selectedHosts);
	}
}

void OnCommandExit(HWND hWndMain)
{
	SNDMSG(hWndMain, WM_CLOSE, 0, 0);
}

void OnCommandAbout(HWND hWndMain)
{
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX),
		hWndMain, AboutDlgProc);
}

void OnCommandHostAdd(HWND hWndMain)
{
	HostViewData data = { 0 };

	INT_PTR dlgResult = DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_HOST_EDIT),
		hWndMain, HostEditDlgProc, (LPARAM)&data);

	if (IDOK == dlgResult) {
		EnableWindow(hWndMain, FALSE);
		SetCursor(LoadCursor(0, IDC_WAIT));

		char* displayName;
		char* hostName;
		char* ipAddr;
		char* macAddr;

#ifdef UNICODE
		displayName = wstr_16_to_8(data.pszDisplayName);
		hostName = wstr_16_to_8(data.pszHostname);
		ipAddr = wstr_16_to_8(data.pszIpAddr);
		macAddr = wstr_16_to_8(data.pszMacAddr);
		HostViewDataFree(&data);
#else
		displayName = data.pszFriendlyName;
		hostName = data.pszHostnaame;
		ipAddr = data.pszIpAddr;
		macAddr = data.pszMacAddr;
#endif

		whost_data_t* hd = whost_data_make(displayName, hostName, ipAddr,
			macAddr, data.tLastSeenOnline);

		wfree(displayName);
		wfree(hostName);
		wfree(ipAddr);
		wfree(macAddr);

		WaolawAddHostToModel(hd);
		whost_data_free(hd);

		SetCursor(LoadCursor(0, IDC_ARROW));
		EnableWindow(hWndMain, TRUE);
	}
}

void OnCommandHostCopy()
{
	uint32_t hostCount;
	HostView** selectedHosts = ListViewGetSelectedHosts(&hostCount);

	if (hostCount > 0)
	{
		size_t nTotalBubLen = 0;
		LPTSTR* hostStrings = walloc(sizeof(LPTSTR) * hostCount);

		for (uint32_t i = hostCount; i --> 0;)
		{
			hostStrings[i] = HostViewToString(selectedHosts[i]);
			nTotalBubLen += _tcslen(hostStrings[i]) + 1;
		}

		wfree(selectedHosts);

		LPTSTR szBuffer = walloc(sizeof(TCHAR) * nTotalBubLen);
		szBuffer[0] = _T('\0');
		LPTSTR pszBufferPointer = szBuffer;

		for (uint32_t i = hostCount; i --> 0;)
		{
			_tcscpy_s(pszBufferPointer, nTotalBubLen - (pszBufferPointer - szBuffer), hostStrings[i]);
			pszBufferPointer += _tcslen(hostStrings[i]);
			wfree(hostStrings[i]);
			if (i > 0)
			{
				*pszBufferPointer++ = _T('\r');
				*pszBufferPointer = _T('\n');
			}
		}

		wfree(hostStrings);

		ClipboardCopy(szBuffer);

		wfree(szBuffer);
	}
}

void OnCommandHostEdit(HWND hWndMain)
{
	HostView* hostView = ListViewGetSelectedHost();
	HostViewData data = { 0 };

	if (hostView) {
		HostViewExportData(hostView, &data);
	}

	INT_PTR dlgResult = DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_HOST_EDIT),
		hWndMain, HostEditDlgProc, (LPARAM)&data);

	if (IDOK == dlgResult) {
		EnableWindow(hWndMain, FALSE);
		SetCursor(LoadCursor(0, IDC_WAIT));

		BOOL isChanged = HostViewImportData(hostView, &data);

		if (isChanged) {
			ListViewUpdateHost(hostView);
			WaolawUpdateHost(hostView);
		}

		SetCursor(LoadCursor(0, IDC_ARROW));
		EnableWindow(hWndMain, TRUE);
	}

	HostViewDataFree(&data);
}

void OnCommandHostDelete(HWND hWndMain)
{
	TCHAR szBuffer[DEF_STR_BUFFER_SIZE];
	LoadString(g_hInst, IDS_HOST_DELETION_CONFIRMATION,
		szBuffer, DEF_STR_BUFFER_SIZE);

	if (MessageBox(hWndMain, szBuffer, _T("WaolaW"),
		MB_YESNO | MB_ICONQUESTION) == IDYES) {
		uint32_t hostCount;
		HostView** selectedHosts = ListViewGetSelectedHosts(&hostCount);
		WaolawDeleteHosts(selectedHosts, hostCount);
		ListViewDeleteHosts(selectedHosts, hostCount);
		DestroyHostViews(selectedHosts, hostCount);
		wfree(selectedHosts);
	}
}

void OnSelectAllHosts()
{
	ListViewSelectAllHosts();
}

void UpdateUI()
{
	wtask_t tasks = WaolawGetCurrentTasks();
	DoUpdateUI(tasks);
}

void DoUpdateUI(wtask_t runningTasks)
{
	int tbbId = wt_discover & runningTasks
		? ID_TBB_STOP_RESCAN
		: ID_TBB_RESCAN;

	ToolBarChangeButtonBitmap(ID_WAOLA_RESCAN, tbbId);

	tbbId = wt_refresh & runningTasks
		? ID_TBB_STOP_REFRESH
		: ID_TBB_REFRESH;

	ToolBarChangeButtonBitmap(ID_WAOLA_REFRESH, tbbId);

	int nSelectedCount = ListViewGetSelectedCount();

	ToolBarEnableButton(ID_WAOLA_WAKE_UP, nSelectedCount > 0);
	ToolBarEnableButton(ID_EDIT_ADDHOST, wt_idle == runningTasks);
	ToolBarEnableButton(ID_EDIT_COPYHOST, nSelectedCount > 0);
	ToolBarEnableButton(ID_EDIT_EDITHOST,
		wt_idle == runningTasks && nSelectedCount == 1);
	ToolBarEnableButton(ID_EDIT_DELETEHOST,
		wt_idle == runningTasks && nSelectedCount > 0);
}

static void DestroyHostViews(HostView** hostViews, uint32_t nCount)
{
	for (int i = nCount; i --> 0;) {
		HostViewDestroy(hostViews[i]);
	}
}
