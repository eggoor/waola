#include "pch.h"

#include "Resource.h"
#include "StatusMonitor.h"

#include "StatusMonitorDiscoverer.h"

extern HINSTANCE g_hInst;
extern wmodule_t g_WaolaModule;
extern int g_OpCode;
extern void* g_StatusData;
extern TCHAR szSatus[STATUS_BUFFER_SIZE];

static BOOL OnDiscovererPassedIpAddress(BOOL bUpdated,
	int opCode, uint32_t ipAddr)
{
	if (!bUpdated && g_StatusData) {
		uint32_t prevIpAddr = *(uint32_t*)g_StatusData;
		bUpdated = prevIpAddr != ipAddr;
	}

	if (bUpdated) {
		UINT nFormatId = DiscovererOpCodeToFormatId(opCode);
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

BOOL ProcessDiscovererStatus(int opCode, const void* const restrict data)
{
	BOOL bUpdated = FALSE;

	if (wm_discoverer != g_WaolaModule) {
		bUpdated = TRUE;
		g_WaolaModule = wm_discoverer;
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
	case wdi_scheduling_arp:
	case wdi_sending_arp:
	case wdi_got_arp:
	case wdi_arp_failed:
		if (IsStatusDataValid(data,
			"Passed NULL data to ProcessDiscovererStatus(wdi_scheduling_arp|wdi_sending_arp|wdi_got_arp|wdi_arp_failed)\n")) {
			bUpdated = OnDiscovererPassedIpAddress(bUpdated, opCode, *(uint32_t*)data);
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

static UINT DiscovererOpCodeToFormatId(int opCode)
{
	UINT nFormatId = 0;

	switch (opCode)
	{
	case wdi_scheduling_arp:
		nFormatId = IDS_STATUS_SCEDULING_ARP;
		break;
	case wdi_sending_arp:
		nFormatId = IDS_STATUS_SENDING_ARP;
		break;
	case wdi_got_arp:
		nFormatId = IDS_STATUS_GOT_ARP;
		break;
	case wdi_arp_failed:
		nFormatId = IDS_STATUS_ARP_FAILED;
		break;
	default:
		nFormatId = IDS_STATUS_ERROR;
		wlog_if_level(wll_warning, "Invalid operation code %d passed to %s\n",
			opCode, __func__);
	}

	return nFormatId;
}
