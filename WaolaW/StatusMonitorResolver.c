#include "pch.h"

#include "Resource.h"
#include "StatusMonitor.h"

#include "StatusMonitorResolver.h"

extern HINSTANCE g_hInst;
extern wmodule_t g_WaolaModule;
extern int g_OpCode;
extern void* g_StatusData;
extern TCHAR szSatus[STATUS_BUFFER_SIZE];

BOOL ProcessResolverStatus(int opCode, const void* const restrict data)
{
	BOOL bUpdated = FALSE;

	if (wm_resolver != g_WaolaModule) {
		bUpdated = TRUE;
		g_WaolaModule = wm_resolver;
		if (g_OpCode != opCode) {
			g_OpCode = opCode;
		}
	}
	else if (g_OpCode != opCode) {
		bUpdated = TRUE;
		g_OpCode = opCode;
	}

	switch (opCode)
	{
	case wre_getaddrinfo:
	case wre_getaddrinfo_failed:
	case wre_gotnameinfo:
	{
		if (IsStatusDataValid(data, "Passed NULL data to ProcessResolverStatus(wre_getaddrinfo(_failed))|wre_gotnameinfo\n")) {
			bUpdated = OnResolverPassedHostname(bUpdated, opCode, data);
		}
		else if (!bUpdated) {
			bUpdated = TRUE;
		}
	}
	break;
	case wre_gotaddrinfo:
	case wre_getnameinfo:
	case wre_getnameinfo_failed:
	case wre_sending_arp:
	case wre_got_arp:
	case wre_arp_failed:
		if (IsStatusDataValid(data,
			"Passed NULL data to ProcessResolverStatus(wre_gotaddrinfo|wre_getnameinfo|wre_getnameinfo_failed|wre_sending_arp|wre_got_arp|wre_arp_failed)\n")) {
			bUpdated = OnResolverPassedIpAddr(bUpdated, opCode, *(uint32_t*)data);
		}
		else if (!bUpdated) {
			bUpdated = TRUE;
		}
		break;
	default:
		LoadString(g_hInst, IDS_STATUS_ERROR,
			szSatus, STATUS_BUFFER_SIZE);
		wlog_if_level(wll_warning, "Invalid operation code %d passed to %s\n",
			opCode, __func__);
	}

	return bUpdated;
}

static UINT ResolverOpCodeToFormatId(int opCode)
{
	UINT nFormatId = 0;

	switch (opCode)
	{
	case wre_getaddrinfo:
		nFormatId = IDS_STATUS_RESOLVER_GETADDRINFO_FORMAT;
		break;
	case wre_gotaddrinfo:
		nFormatId = IDS_STATUS_RESOLVER_GOTADDRINFO_FORMAT;
		break;
	case wre_getaddrinfo_failed:
		nFormatId = IDS_STATUS_RESOLVER_GETADDRINFO_FAILED_FORMAT;
		break;
	case wre_getnameinfo:
		nFormatId = IDS_STATUS_RESOLVER_GETNAMEINFO_FORMAT;
		break;
	case wre_gotnameinfo:
		nFormatId = IDS_STATUS_RESOLVER_GOTNAMEINFO_FORMAT;
		break;
	case wre_getnameinfo_failed:
		nFormatId = IDS_STATUS_RESOLVER_GETNAMEINFO_FAILED_FORMAT;
		break;
	case wre_sending_arp:
		nFormatId = IDS_STATUS_SENDING_ARP;
		break;
	case wre_got_arp:
		nFormatId = IDS_STATUS_GOT_ARP;
		break;
	case wre_arp_failed:
		nFormatId = IDS_STATUS_ARP_FAILED;
		break;
	default:
		nFormatId = IDS_STATUS_ERROR;
		wlog_if_level(wll_warning, "Invalid operation code %d passed to %s\n",
			opCode, __func__);
	}

	return nFormatId;
}

static BOOL OnResolverPassedHostname(BOOL bUpdated,
	int opCode, const char* const restrict hostname)
{
	if (!bUpdated && g_StatusData) {
		const char* prevHostname = g_StatusData;
		bUpdated = strcmp(prevHostname, hostname) == 0;
	}

	if (bUpdated) {
		UINT nFormatId = ResolverOpCodeToFormatId(opCode);
		if (IDS_STATUS_ERROR != nFormatId) {
			ProcessResolverHostname(hostname, nFormatId);
		}
		else {
			LoadString(g_hInst, nFormatId,
				szSatus, STATUS_BUFFER_SIZE);
		}
	}

	return bUpdated;
}

static BOOL OnResolverPassedIpAddr(BOOL bUpdated,
	int opCode, uint32_t ipAddr)
{
	if (!bUpdated && g_StatusData) {
		uint32_t prevIpAddr = *(uint32_t*)g_StatusData;
		bUpdated = prevIpAddr != ipAddr;
	}

	if (bUpdated) {
		UINT nFormatId = ResolverOpCodeToFormatId(opCode);
		if (IDS_STATUS_ERROR != nFormatId) {
			ProcessIpAddress(ipAddr, nFormatId);
		}
		else {
			LoadString(g_hInst, nFormatId,
				szSatus, STATUS_BUFFER_SIZE);
		}
	}

	return bUpdated;
}

static BOOL OnResolverGetNameInfo(BOOL bUpdated,
	uint32_t ipAddr, UINT nFormatId)
{
	if (!bUpdated && g_StatusData) {
		uint32_t prevIpAddr = *(uint32_t*)g_StatusData;
		bUpdated = prevIpAddr != ipAddr;
	}

	if (bUpdated) {
		ProcessIpAddress(ipAddr, nFormatId);
	}

	return bUpdated;
}

static void ProcessResolverHostname(const char* hostname,
	UINT nFormatId)
{
	wfree(g_StatusData);
	g_StatusData = wstrdup(hostname);
	LPTSTR pszHostname = NULL;

#ifdef UNICODE
	pszHostname = wstr_8_to_16(hostname);
#else
	pszHostname = hostname;
#endif

	TCHAR szFormat[DEF_STR_BUFFER_SIZE];
	LoadString(g_hInst, nFormatId,
		szFormat, DEF_STR_BUFFER_SIZE);
	_sntprintf_s(szSatus, STATUS_BUFFER_SIZE,
		_TRUNCATE, szFormat, pszHostname);

#ifdef UNICODE
	wfree(pszHostname);
#endif
}
