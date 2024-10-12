#include "pch.h"

#include "Resource.h"
#include "WindowPos.h"
#include "RichEdit.h"
#include "Clipboard.h"

#include "DlgAbout.h"

extern HINSTANCE g_hInst;

static const int g_nTabCount = 3;
static int selectedItem = -1;
static HMODULE g_hMsFtEditModule;

void OnWmInitDialog(HWND hDlg)
{
	InitTabCtrl(hDlg);
	CenterWindow(GetParent(hDlg), hDlg);
}

INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			selectedItem = -1;
			return (INT_PTR)TRUE;
		}
		break;
	case WM_INITDIALOG:
	{
		OnWmInitDialog(hDlg);
		return (INT_PTR)TRUE;
	}
	case WM_MOVE:
	{
		if (-1 == selectedItem)
		{
			selectedItem = 0;
			AdjustTabCtrlSelectedChildPos(GetDlgItem(hDlg, IDC_TAB1));
		}
	}
	break;
	case WM_NOTIFY:
	{
		const LPNMHDR pNmHdr = (LPNMHDR)lParam;

		switch (pNmHdr->idFrom)
		{
		case IDC_TAB1:
		{
			OnTabEvent(hDlg, pNmHdr->code);
		}
		break;
		case IDC_RICH_EDIT_ABOUT:
		case IDC_RICH_EDIT_CREDITS:
		case IDC_RICH_EDIT_LICENSE:
		{
			OnRichEditEvent((const ENLINK*)pNmHdr);
		}
		break;
		}

		return 0;
	}
	break;
	case WM_CONTEXTMENU:
	{
		HWND hwndTabCtrl = (HWND)wParam;
		TC_ITEM tcItem = {
			.mask = TCIF_PARAM
		};
		TabCtrl_GetItem(hwndTabCtrl, selectedItem, &tcItem);

		OnContextMenu((HWND)tcItem.lParam, lParam);
		return TRUE;
	}
	break;
	}
	return (INT_PTR)FALSE;
}

void InitTabCtrl(HWND hDlg)
{
	if (!g_hMsFtEditModule)
	{
		g_hMsFtEditModule = LoadLibrary(_T("Msftedit.dll"));
	}

	HWND hwndTabCtrl = GetDlgItem(hDlg, IDC_TAB1);

	TCHAR szTextBuffer[DEF_STR_BUFFER_SIZE];
	LoadString(g_hInst, IDS_ABOUT, szTextBuffer, DEF_STR_BUFFER_SIZE);

	DWORD dwTabChildStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE
		| ES_READONLY | WS_VSCROLL;

	HWND hwndChild = RichEditMake(hwndTabCtrl, IDC_RICH_EDIT_ABOUT,
		szTextBuffer, dwTabChildStyle);
	FillRichEditFromResource(hwndChild, IDR_ABOUT);

	TCITEM tcItem = {
		.mask = TCIF_TEXT | TCIF_PARAM,
		.iImage = -1,
		.pszText = szTextBuffer,
		.lParam = (LPARAM)hwndChild
	};

	TabCtrl_InsertItem(hwndTabCtrl, 0, &tcItem);

	dwTabChildStyle &= ~WS_VISIBLE;

	LoadString(g_hInst, IDS_CREDITS, szTextBuffer, DEF_STR_BUFFER_SIZE);
	hwndChild = RichEditMake(hwndTabCtrl, IDC_RICH_EDIT_CREDITS,
		szTextBuffer, dwTabChildStyle);
	FillRichEditFromResource(hwndChild, IDR_CREDITS);

	tcItem.lParam = (LPARAM)hwndChild;
	TabCtrl_InsertItem(hwndTabCtrl, 1, &tcItem);

	LoadString(g_hInst, IDS_LICENSE, szTextBuffer, DEF_STR_BUFFER_SIZE);
	hwndChild = RichEditMake(hwndTabCtrl, IDC_RICH_EDIT_LICENSE,
		szTextBuffer, dwTabChildStyle);
	FillRichEditFromResource(hwndChild, IDR_LICENSE);
	tcItem.lParam = (LPARAM)hwndChild;
	TabCtrl_InsertItem(hwndTabCtrl, 2, &tcItem);
}

void AdjustTabCtrlSelectedChildPos(HWND hwndTabCtrl)
{
	TC_ITEM tcItem = {
		.mask = TCIF_PARAM
	};

	TabCtrl_GetItem(hwndTabCtrl, selectedItem, &tcItem);

	if (0 == tcItem.lParam)
	{
		return;
	}

	RECT rcTab;
	GetClientRect(hwndTabCtrl, &rcTab);
	TabCtrl_AdjustRect(hwndTabCtrl, FALSE, &rcTab);

	SIZE sizeTab = {
		.cx = rcTab.right - rcTab.left,
		.cy = rcTab.bottom - rcTab.top
	};

	SetWindowPos((HWND)tcItem.lParam, NULL, rcTab.left, rcTab.top,
		sizeTab.cx, sizeTab.cy, SWP_NOZORDER);
}

void OnTabEvent(HWND hDlg, UINT opCode)
{
	switch (opCode)
	{
	case TCN_SELCHANGE:
	{
		HWND hwndTabCtrl = GetDlgItem(hDlg, IDC_TAB1);
		TC_ITEM tcItem = {
			.mask = TCIF_PARAM
		};

		TabCtrl_GetItem(hwndTabCtrl, selectedItem, &tcItem);

		if (0 == tcItem.lParam)
		{
			break;
		}

		ShowWindow((HWND)tcItem.lParam, SW_HIDE);

		selectedItem = TabCtrl_GetCurSel(hwndTabCtrl);
		TabCtrl_GetItem(hwndTabCtrl, selectedItem, &tcItem);

		AdjustTabCtrlSelectedChildPos(hwndTabCtrl);
		ShowWindow((HWND)tcItem.lParam, SW_SHOW);
		UpdateWindow((HWND)tcItem.lParam);
	}
	}
}

void OnRichEditEvent(const ENLINK* pLink)
{
	if (WM_LBUTTONUP == pLink->msg)
	{
		LPTSTR pszUrl = RichEditGetTextRange(pLink->nmhdr.hwndFrom, pLink->chrg);
		OpenURL(pszUrl);
		wfree(pszUrl);
	}
}

void OnContextMenu(HWND hWnd, LPARAM lParam)
{
	HMENU hMenu = CreatePopupMenu();
	TCHAR szTextBuffer[DEF_STR_BUFFER_SIZE];
	LoadString(g_hInst, IDS_COPY, szTextBuffer, DEF_STR_BUFFER_SIZE);

	InsertMenu(hMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, ID_COPY_SELECTED_TEXT, szTextBuffer);
	UINT_PTR menuId = TrackPopupMenu(hMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD,
		GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, hWnd, NULL);

	if (ID_COPY_SELECTED_TEXT == menuId)
	{
		LPTSTR pszSelection = RichEditGetSelectedText(hWnd);
		ClipboardCopy(pszSelection);
		wfree(pszSelection);
	}
}
