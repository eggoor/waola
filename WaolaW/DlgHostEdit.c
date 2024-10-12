#include "pch.h"

#include "Resource.h"
#include "WindowPos.h"

#include "DlgHostEdit.h"

extern HINSTANCE g_hInst;

static HostViewData* g_HostEditDlgData;

void OnWmInitDialog(HWND hDlg, HostViewData* const restrict data)
{
	g_HostEditDlgData = data;

	LoadData(hDlg);

	CenterWindow(GetParent(hDlg), hDlg);
}

INT_PTR CALLBACK HostEditDlgProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		OnWmInitDialog(hDlg, (HostViewData*)lParam);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK)
			{
				SaveData(hDlg);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

static void LoadData(HWND hDlg)
{
	if (!HostViewDataIsEmpty(g_HostEditDlgData))
	{
		TCHAR szTextBuffer[DEF_STR_BUFFER_SIZE];
		LoadString(g_hInst, IDS_TT_HOST_EDIT, szTextBuffer,
			DEF_STR_BUFFER_SIZE);

		SetWindowText(hDlg, szTextBuffer);

		SetDlgItemText(hDlg, IDC_EDIT_FRIENDLY_NAME,
			g_HostEditDlgData->pszDisplayName);
		SetDlgItemText(hDlg, IDC_EDIT_HOSTNAME,
			g_HostEditDlgData->pszHostname);
		SetDlgItemText(hDlg, IDC_EDIT_IP_ADDR,
			g_HostEditDlgData->pszIpAddr);
		SetDlgItemText(hDlg, IDC_EDIT_MAC_ADDR,
			g_HostEditDlgData->pszMacAddr);
		HostViewDataFree(g_HostEditDlgData);
	}
}

static void SaveData(HWND hDlg)
{
	TCHAR szBuffer[DEF_STR_BUFFER_SIZE];

	GetDlgItemText(hDlg, IDC_EDIT_FRIENDLY_NAME,
		szBuffer, DEF_STR_BUFFER_SIZE);

	g_HostEditDlgData->pszDisplayName = StrDup(szBuffer);

	GetDlgItemText(hDlg, IDC_EDIT_HOSTNAME,
		szBuffer, DEF_STR_BUFFER_SIZE);
	g_HostEditDlgData->pszHostname = StrDup(szBuffer);

	GetDlgItemText(hDlg, IDC_EDIT_IP_ADDR,
		szBuffer, DEF_STR_BUFFER_SIZE);
	g_HostEditDlgData->pszIpAddr = StrDup(szBuffer);

	GetDlgItemText(hDlg, IDC_EDIT_MAC_ADDR,
		szBuffer, DEF_STR_BUFFER_SIZE);
	g_HostEditDlgData->pszMacAddr = StrDup(szBuffer);

	g_HostEditDlgData->tLastSeenOnline = 0;
}
