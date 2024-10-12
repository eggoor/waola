#include "pch.h"

#include "PermanentStorage.h"

extern HINSTANCE g_hInst;

static LPCTSTR g_sBaseKey = _T("Software\\eSoft\\Waola");
static LPCTSTR g_sVersionHi = _T("VersionHi");
static LPCTSTR g_sVersionLo = _T("VersionLo");
static LPCTSTR g_sHosts = _T("Hosts");
static LPCTSTR g_sDisplayName = _T("DisplayName");
static LPCTSTR g_sHostname = _T("Hostname");
static LPCTSTR g_sIpAddr = _T("IpAddr");
static LPCTSTR g_sMacAddr = _T("MacAddr");
static LPCTSTR g_sLastSeenOnline = _T("LastSeenOnline");
static LPCTSTR g_sWndPos = _T("WindowPos");
static LPCTSTR g_sLeft = _T("Left");
static LPCTSTR g_sTop = _T("Top");
static LPCTSTR g_sRight = _T("Right");
static LPCTSTR g_sBottom = _T("Bottom");

void SaveHosts(HDPA hostCollection)
{
	HKEY hBase;
	LSTATUS retVal = RegCreateKeyEx(HKEY_CURRENT_USER, g_sBaseKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hBase, NULL);

	if (ERROR_SUCCESS != retVal) {
		return;
	}

	LPVOID pResCopyBuffer;

	VS_FIXEDFILEINFO* lpFfi = GetVersionInfo(&pResCopyBuffer);

	RegSetValueEx(hBase, g_sVersionHi, 0, REG_DWORD, (const byte*)&lpFfi->dwProductVersionMS,
		(int)(sizeof(lpFfi->dwProductVersionMS)));
	RegSetValueEx(hBase, g_sVersionLo, 0, REG_DWORD, (const byte*)&lpFfi->dwProductVersionLS,
		(int)(sizeof(lpFfi->dwProductVersionLS)));

	wfree(pResCopyBuffer);

	HKEY hHosts;
	retVal = RegCreateKeyEx(hBase, g_sHosts, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hHosts, NULL);
	if (ERROR_SUCCESS != retVal) {
		goto cleanup;
	}

	PermanentStorageCbd cbd = {
		.hHosts = hHosts
	};

	EnumHosts(DeleteHostCb, &cbd);

	wlog_if_level(wll_info, "Gonna save %d hosts\n",
		DPA_GetPtrCount(hostCollection));

	for (int i = DPA_GetPtrCount(hostCollection); i --> 0;) {
		HostView* hostView = DPA_FastGetPtr(hostCollection, i);
		SaveHost(hHosts, i, hostView);
	}

	RegCloseKey(hHosts);

cleanup:
	RegCloseKey(hBase);
}

HDPA LoadHosts()
{
	HDPA hostDataList = DPA_Create(4);

	HKEY hBase;
	LSTATUS retVal = RegCreateKeyEx(HKEY_CURRENT_USER, g_sBaseKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &hBase, NULL);
	if (ERROR_SUCCESS != retVal) {
		goto end;
	}

	HKEY hHosts;
	retVal = RegCreateKeyEx(hBase, g_sHosts, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hHosts, NULL);
	if (ERROR_SUCCESS != retVal) {
		goto cleanup;
	}

	PermanentStorageCbd cbd = {
		.hHosts = hHosts,
		.hostDataList = hostDataList
	};

	EnumHosts(AddHostCb, &cbd);

	RegCloseKey(hHosts);

cleanup:
	RegCloseKey(hBase);

end:
	return hostDataList;
}

static VS_FIXEDFILEINFO* GetVersionInfo(LPVOID* ppResCopyBuffer)
{
	VS_FIXEDFILEINFO* lpFfi = NULL;

	if (!ppResCopyBuffer) {
		goto end;
	}
	
	*ppResCopyBuffer = NULL;

	HRSRC hResInfo = FindResource(g_hInst, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);

	if (!hResInfo) {
		goto end;
	}

	DWORD dwSize = SizeofResource(g_hInst, hResInfo);
	HGLOBAL hResData = LoadResource(g_hInst, hResInfo);

	if (!hResData) {
		goto end;
	}

	LPVOID pRes = LockResource(hResData);
	*ppResCopyBuffer = walloc(dwSize);
	memcpy(*ppResCopyBuffer, pRes, dwSize);
	FreeResource(hResData);

	UINT uLen;
	VerQueryValue(*ppResCopyBuffer, TEXT("\\"), (LPVOID*)&lpFfi, &uLen);

end:
	return lpFfi;
}

void SaveWindowPos(const RECT* rcPos)
{
	HKEY hBase;
	LSTATUS retVal = RegCreateKeyEx(HKEY_CURRENT_USER, g_sBaseKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hBase, NULL);

	if (ERROR_SUCCESS != retVal) {
		return;
	}

	HKEY hWndPos;
	retVal = RegCreateKeyEx(hBase, g_sWndPos, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hWndPos, NULL);
	if (ERROR_SUCCESS != retVal) {
		goto cleanup;
	}

	RegSetValueEx(hWndPos, g_sLeft, 0, REG_DWORD, (const byte*)&rcPos->left,
		(int)(sizeof(rcPos->left)));
	RegSetValueEx(hWndPos, g_sTop, 0, REG_DWORD, (const byte*)&rcPos->top,
		(int)(sizeof(rcPos->top)));
	RegSetValueEx(hWndPos, g_sRight, 0, REG_DWORD, (const byte*)&rcPos->right,
		(int)(sizeof(rcPos->right)));
	RegSetValueEx(hWndPos, g_sBottom, 0, REG_DWORD, (const byte*)&rcPos->bottom,
		(int)(sizeof(rcPos->bottom)));

	RegCloseKey(hWndPos);

cleanup:
	RegCloseKey(hBase);
}

BOOL LoadWindowPos(RECT* rcPos)
{
	BOOL loaded = FALSE;

	HKEY hBase;
	LSTATUS retVal = RegCreateKeyEx(HKEY_CURRENT_USER, g_sBaseKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &hBase, NULL);
	if (ERROR_SUCCESS != retVal) {
		goto end;
	}

	HKEY hWndPos;
	retVal = RegCreateKeyEx(hBase, g_sWndPos, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hWndPos, NULL);
	if (ERROR_SUCCESS != retVal) {
		goto cleanup1;
	}

	DWORD nBuffSize = sizeof(rcPos->left);

	retVal = RegGetValue(hWndPos, NULL, g_sLeft, RRF_RT_DWORD,
		NULL, &rcPos->left, &nBuffSize);

	if (ERROR_SUCCESS != retVal) {
		goto cleanup2;
	}
	
	nBuffSize = sizeof(rcPos->top);
	retVal = RegGetValue(hWndPos, NULL, g_sTop, RRF_RT_DWORD,
		NULL, &rcPos->top, &nBuffSize);

	if (ERROR_SUCCESS != retVal) {
		goto cleanup2;
	}

	nBuffSize = sizeof(rcPos->right);
	retVal = RegGetValue(hWndPos, NULL, g_sRight, RRF_RT_DWORD,
		NULL, &rcPos->right, &nBuffSize);

	if (ERROR_SUCCESS != retVal) {
		goto cleanup2;
	}

	nBuffSize = sizeof(rcPos->bottom);
	retVal = RegGetValue(hWndPos, NULL, g_sBottom, RRF_RT_DWORD,
		NULL, &rcPos->bottom, &nBuffSize);

	if (ERROR_SUCCESS != retVal) {
		goto cleanup2;
	}

	loaded = TRUE;

cleanup2:
	RegCloseKey(hWndPos);

cleanup1:
	RegCloseKey(hBase);

end:
	return loaded;
}

static void SaveHost(HKEY hParent, int nNumber, const HostView* const restrict hostView)
{
	HKEY hHost;

	TCHAR buffer[16];
	_itot_s(nNumber, buffer, 16, 10);

	LSTATUS retVal = RegCreateKeyEx(hParent, buffer, 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hHost, NULL);

	if (ERROR_SUCCESS != retVal) {
		return;
	}

	LPCTSTR pszValue = HostViewGetValue(hostView, hwf_host_name);
	if (pszValue) {
		RegSetValueEx(hHost, g_sHostname, 0, REG_SZ, (const byte*)pszValue,
			(int)(sizeof(TCHAR) * (_tcslen(pszValue) + 1)));
	}

	LPCTSTR pszDisplayName = HostViewGetValue(hostView, hwf_display_name);
	if (pszDisplayName && pszValue && _tcscmp(pszDisplayName, pszValue) != 0) {
		RegSetValueEx(hHost, g_sDisplayName, 0, REG_SZ, (const byte*)pszDisplayName,
			(int)(sizeof(TCHAR) * (_tcslen(pszDisplayName) + 1)));
	}

	pszValue = HostViewGetValue(hostView, hwf_ip_address);
	if (pszValue) {
		RegSetValueEx(hHost, g_sIpAddr, 0, REG_SZ, (const byte*)pszValue,
			(int)(sizeof(TCHAR) * (_tcslen(pszValue) + 1)));
	}

	pszValue = HostViewGetValue(hostView, hwf_mac_address);
	if (pszValue) {
		RegSetValueEx(hHost, g_sMacAddr, 0, REG_SZ, (const byte*)pszValue,
			(int)(sizeof(TCHAR) * (_tcslen(pszValue) + 1)));
	}

	const whost_t* host = whost_view_get_host(HostViewGetHostView(hostView));
	time_t lso = whost_get_last_seen_online(host);

	retVal = RegSetValueEx(hHost, g_sLastSeenOnline, 0, REG_QWORD,
		(const byte*)&lso, (int)(sizeof(lso)));

	RegCloseKey(hHost);
}

static whost_data_t* LoadHost(HKEY hHosts, LPCTSTR pszHostId)
{
	whost_data_t* hd = NULL;

	HostViewData hostViewData = { NULL };

	HKEY hHost;
	LSTATUS retVal = RegCreateKeyEx(hHosts, pszHostId, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &hHost, NULL);

	if (ERROR_SUCCESS != retVal) {
		goto end;
	}

	TCHAR szBuffer[DEF_STR_BUFFER_SIZE];
	DWORD nBuffSize = sizeof(TCHAR) * DEF_STR_BUFFER_SIZE;

	retVal = RegGetValue(hHost, NULL, g_sDisplayName,
		RRF_RT_REG_SZ, NULL, szBuffer, &nBuffSize);

	hostViewData.pszDisplayName = ERROR_SUCCESS == retVal
		? StrDup(szBuffer)
		: NULL;

	nBuffSize = sizeof(TCHAR) * DEF_STR_BUFFER_SIZE;
	retVal = RegGetValue(hHost, NULL, g_sHostname,
		RRF_RT_REG_SZ, NULL, szBuffer, &nBuffSize);

	hostViewData.pszHostname = ERROR_SUCCESS == retVal
		? StrDup(szBuffer)
		: NULL;

	nBuffSize = sizeof(TCHAR) * DEF_STR_BUFFER_SIZE;
	retVal = RegGetValue(hHost, NULL, g_sIpAddr,
		RRF_RT_REG_SZ, NULL, szBuffer, &nBuffSize);

	hostViewData.pszIpAddr = ERROR_SUCCESS == retVal
		? StrDup(szBuffer)
		: NULL;

	nBuffSize = sizeof(TCHAR) * DEF_STR_BUFFER_SIZE;
	retVal = RegGetValue(hHost, NULL, g_sMacAddr,
		RRF_RT_REG_SZ, NULL, szBuffer, &nBuffSize);

	hostViewData.pszMacAddr = ERROR_SUCCESS == retVal
		? StrDup(szBuffer)
		: NULL;

	nBuffSize = sizeof(hostViewData.tLastSeenOnline);
	RegGetValue(hHost, NULL, g_sLastSeenOnline, RRF_RT_QWORD,
		NULL, &hostViewData.tLastSeenOnline, &nBuffSize);

	RegCloseKey(hHost);

	char* displayName = NULL;
	char* hostName = NULL;
	char* ipAddr = NULL;
	char* macAddr = NULL;

end:

#ifdef UNICODE
	displayName = wstr_16_to_8(hostViewData.pszDisplayName);
	hostName = wstr_16_to_8(hostViewData.pszHostname);
	ipAddr = wstr_16_to_8(hostViewData.pszIpAddr);
	macAddr = wstr_16_to_8(hostViewData.pszMacAddr);

	hd = whost_data_make(displayName, hostName, ipAddr,
		macAddr, hostViewData.tLastSeenOnline);

	wfree(displayName);
	wfree(hostName);
	wfree(ipAddr);
	wfree(macAddr);
#else
	hd = whost_data_make(hostViewData.pszFriendlyName, hostViewData.pszHostname,
		hostViewData.pszIpAddr, hostViewData.pszMacAddr, hostViewData.tLastSeenOnline);
#endif
	HostViewDataFree(&hostViewData);

	return hd;
}

static void EnumHosts(PermanentStorageCb cb,
	PermanentStorageCbd* const restrict cbd)
{
	LPTSTR pszHostId = NULL;
	DWORD bufferSize = 0;

	DWORD subkeyCount = 0;
	LSTATUS retVal = RegQueryInfoKey(cbd->hHosts, NULL, NULL, NULL, &subkeyCount,
		&bufferSize, NULL, NULL, NULL, NULL, NULL, NULL);
	if (ERROR_SUCCESS != retVal) {
		return;
	}

	wlog_if_level(wll_verbose, "%s: Found %d saved hosts\n",
		__func__, subkeyCount);

	if (subkeyCount > 0) {

		bufferSize *= sizeof(TCHAR);
		DWORD cchName = bufferSize;

		pszHostId = walloc(bufferSize);

		size_t bufSize = subkeyCount * sizeof(LPTSTR);
		LPTSTR* subKeys = walloc(bufSize);

		if (!subKeys) {
			wlog(wll_warning, "%s: No %zuB of free memory for regidtry subkey array\n",
				__func__, subkeyCount * sizeof(LPTSTR));
			wfree(pszHostId);
			return;
		}

		DWORD i = 0;
		while ((retVal = RegEnumKeyEx(cbd->hHosts, --subkeyCount, pszHostId,
			&cchName, NULL, NULL, NULL, NULL)) == ERROR_SUCCESS)
		{
			subKeys[i++] = StrDup(pszHostId);
			cchName = bufferSize;
		}

		wfree(pszHostId);

		while (i --> 0) {
			cbd->pszHostId = subKeys[i];
			(*cb)(cbd);
			wfree(subKeys[i]);
		}

		wfree(subKeys);
	}
}

static void AddHostCb(PermanentStorageCbd* const restrict cbd)
{
	wlog_if_level(wll_debug, "%s: Load '%s'\n",
		__func__, cbd->pszHostId);

	whost_data_t* hd = LoadHost(cbd->hHosts, cbd->pszHostId);
	DPA_AppendPtr(cbd->hostDataList, hd);
}

static void DeleteHostCb(PermanentStorageCbd* const restrict cbd)
{
	wlog_if_level(wll_debug, "%s: Delete '%s'\n",
		__func__, cbd->pszHostId);

	RegDeleteKey(cbd->hHosts, cbd->pszHostId);
}
