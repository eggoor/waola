#include "pch.h"

#include "Resource.h"
#include "StatusMonitor.h"

#include "StatusMonitorScanner.h"

extern HINSTANCE g_hInst;
extern wmodule_t g_WaolaModule;
extern int g_OpCode;
extern void* g_StatusData;
extern TCHAR szSatus[STATUS_BUFFER_SIZE];

BOOL ProcessScannerStatus(waolasc_opcode_t opCode)
{
	BOOL bUpdated = FALSE;
	static UINT nStatusStringId;

	if (wm_scanner != g_WaolaModule) {
		bUpdated = TRUE;
		g_WaolaModule = wm_scanner;
		if (g_OpCode != opCode) {
			g_OpCode = opCode;
		}
	}
	else if (g_OpCode != opCode) {
		bUpdated = TRUE;
		g_OpCode = opCode;
	}

	if (bUpdated) {
		switch (opCode)
		{
		case wco_idle:
			nStatusStringId = IDS_STATUS_SCANER_IDLE;
			break;
		case wco_discovering:
			nStatusStringId = IDS_STATUS_SCANER_DISCOVERING;
			break;
		case wco_resolving:
			nStatusStringId = IDS_STATUS_SCANER_RESOLVING;
			break;
		case wco_cancelling:
			nStatusStringId = IDS_STATUS_SCANER_CANCELLING;
			break;
		default:
			nStatusStringId = IDS_STATUS_ERROR;
			wlog_if_level(wll_warning, "Unknown operation code %d passed to %s\n",
				opCode, __func__);
		}

		LoadString(g_hInst, nStatusStringId,
			szSatus, STATUS_BUFFER_SIZE);
	}

	return bUpdated;
}
