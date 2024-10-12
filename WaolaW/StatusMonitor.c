#include "pch.h"

#include "Resource.h"
#include "StatusMonitorScanner.h"
#include "StatusMonitorDiscoverer.h"
#include "StatusMonitorResolver.h"

#include "StatusMonitor.h"

#define MONITOR_FREQ 25

extern HINSTANCE g_hInst;
extern HWND g_hwndStatusBar;

wmodule_t g_WaolaModule;
int g_OpCode;
void* g_StatusData;
TCHAR szSatus[STATUS_BUFFER_SIZE];

static HANDLE g_hThread;
static BOOL g_Cancelled;

void StatusStartMonitoring(waolasc_t* scanner)
{
	if (g_hThread) {
		return;
	}

	g_Cancelled = FALSE;
	g_hThread = DoStartMonitoring(scanner);
}

void StatusStopMonitoring()
{
	g_Cancelled = TRUE;

	if (g_hThread) {
		WaitForSingleObject(g_hThread, 5000);
		CloseHandle(g_hThread);
		g_hThread = NULL;
		wfree(g_StatusData);
		g_WaolaModule = wm_undefined;
		g_OpCode = 0;
		g_StatusData = NULL;
	}
}

static DWORD CALLBACK StatusMonitorProc(LPVOID lpParam)
{
	static const int nMonitorTimeout = 1000 / MONITOR_FREQ;
	const waolasc_t* scanner = lpParam;

	while (!g_Cancelled) {
		UpdateStatus(scanner);
		Sleep(nMonitorTimeout);
		if (g_Cancelled) {
			lpParam = 0;
		}
	}

	UpdateStatus(scanner);

	return 0;
}

static void UpdateStatus(const waolasc_t* scanner)
{
	static const WPARAM wParam = MAKEWPARAM(MAKEWORD(SB_SIMPLEID, 0), 0);

	wmodule_t module;
	int opCode;
	const void* data = waolasc_get_status(scanner,
		&module, &opCode);

	BOOL bStatusUpdated = FALSE;
	if (module != wm_undefined && opCode != 0)
	{
		bStatusUpdated = ProcessStatus(module, opCode, data);
	}

	if (bStatusUpdated) {
		PostMessage(g_hwndStatusBar, SB_SETTEXT,
			wParam, (LPARAM)szSatus);
	}
}

static BOOL ProcessStatus(wmodule_t module,
	int opCode, const void* const restrict data)
{
	BOOL bUpdated = FALSE;

	switch (module)
	{
	case wm_scanner:
		bUpdated = ProcessScannerStatus(opCode);
		break;
	case wm_discoverer:
		bUpdated = ProcessDiscovererStatus(opCode, data);
		break;
	case wm_resolver:
		bUpdated = ProcessResolverStatus(opCode, data);
		break;
	default:
		wlog_if_level(wll_warning, "Invalid waola module identifier %d passed to %s\n",
			module, __func__);
	}

	return bUpdated;
}

static HANDLE DoStartMonitoring(waolasc_t* scanner)
{
	HANDLE hThread = CreateThread(
		NULL,						// default security attributes
		0,							// use default stack size	
		StatusMonitorProc,			// thread function name
		scanner,					// argument to thread function 
		0,							// use default creation flags 
		NULL);						// returns the thread identifier

	return hThread;
}

BOOL IsStatusDataValid(const void* const restrict data, const char* const restrict warningMsg)
{
	if (!data) {
		wlog_if_level(wll_warning, warningMsg);
		LoadString(g_hInst, IDS_STATUS_ERROR,
			szSatus, STATUS_BUFFER_SIZE);
	}

	return !!data;
}

void ProcessIpAddress(uint32_t ipAddr, UINT nFormatId)
{
	wfree(g_StatusData);
	g_StatusData = NULL;
	char saBuffer[INET6_ADDRSTRLEN];
	const char* ip_addr_str = wsockaddr_int_to_string(ipAddr,
		saBuffer, INET6_ADDRSTRLEN);

	if (!ip_addr_str) {
		wlog_if_level(wll_warning, "wsockaddr_int_to_string failed");
		LoadString(g_hInst, IDS_STATUS_ERROR,
			szSatus, STATUS_BUFFER_SIZE);
		return;
	}

	g_StatusData = walloc(sizeof(uint32_t));
	if (g_StatusData) {
		*(uint32_t*)g_StatusData = ipAddr;
	}
	else {
		wlog_if_level(wll_warning, "No %zuB of free memory for IPv4 address\n",
			sizeof(uint32_t));
	}

	LPTSTR pszIpAddr = NULL;

#ifdef UNICODE
	pszIpAddr = wstr_8_to_16(ip_addr_str);
#else
	pszIpAddr = ip_addr_str;
#endif

	TCHAR szFormat[DEF_STR_BUFFER_SIZE];
	LoadString(g_hInst, nFormatId, szFormat, DEF_STR_BUFFER_SIZE);
	_sntprintf_s(szSatus, STATUS_BUFFER_SIZE,
		_TRUNCATE, szFormat, pszIpAddr);

#ifdef UNICODE
	wfree(pszIpAddr);
#endif
}
