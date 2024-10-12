#include "pch.h"

#include "Resource.h"
#include "WaolaW.h"
#include "CommandHandlers.h"
#include "PermanentStorage.h"
#include "WindowPos.h"
#include "ToolBar.h"
#include "ListView.h"
#include "StatusBar.h"

#include "MessageHandlers.h"

extern HINSTANCE g_hInst;
extern HWND g_hwndToolBar;
extern HWND g_hwndListView;
extern HWND g_hwndStatusBar;


void OnWmCommand(HWND hWndMain, WPARAM wParam, LPARAM lParam)
{
	const int nCommandId = GET_WM_COMMAND_ID(wParam, lParam);

	switch (nCommandId)
	{
	case ID_WAOLA_RESCAN:
	case ID_WAOLA_REFRESH:
	case ID_WAOLA_ADD:
		OnCommandWaolaTask(nCommandId, hWndMain);
		break;
	case ID_WAOLA_WAKE_UP:
		OnCommandWakeUp();
		break;
	case ID_WAOLA_WAKE_UP_SELECTED:
		OnCommandWakeUpSelected();
		break;
	case ID_EDIT_ADDHOST:
		OnCommandHostAdd(hWndMain);
		break;
	case ID_EDIT_COPYHOST:
		OnCommandHostCopy();
		break;
	case ID_EDIT_EDITHOST:
		OnCommandHostEdit(hWndMain);
		break;
	case ID_EDIT_DELETEHOST:
		OnCommandHostDelete(hWndMain);
		break;
	case ID_ACCEL_SELECT_ALL_HOSTS:
		OnSelectAllHosts();
		break;
	case IDM_EXIT:
		OnCommandExit(hWndMain);
		break;
	case IDM_ABOUT:
		OnCommandAbout(hWndMain);
		break;
	}
}

void OnWmWaolaStateChanged(wtask_t tasks)
{
	WaolaProcessStateChanged(tasks);
}

void OnWmWaolaHostDiscovered(waolasc_opcode_t op, whost_view_t* hw)
{
	WaolaProcessHostDiscovered(op, hw);
}

void OnWmWaolaTaskFinished(HWND hWndMain, UINT_PTR rc)
{
	WaolawOnTaskFinished(hWndMain);
	SetCursor(LoadCursor(0, IDC_ARROW));
	
	if (rc) {
		MessageBox(NULL, L"Task failed",
			TEXT("Error"), MB_ICONSTOP | MB_OK);
	}
}

BOOL OnWmSetCursor()
{
	if (WaolawGetCurrentTasks() != wt_idle) {
		SetCursor(LoadCursor(0, IDC_APPSTARTING));
		return TRUE;
	}

	return FALSE;
}

void OnWmCreate(HWND hWndMain)
{
	RECT rcMain;
	if (LoadWindowPos(&rcMain)) {
		AdjustWindowPos(hWndMain, &rcMain);
	}
	else {
		CenterWindow(GetDesktopWindow(), hWndMain);
	}

	g_hwndToolBar = ToolBarCreate(hWndMain);
	if (!g_hwndToolBar) {
		wlog_if_level(wll_fatal, "ToolBarCreate failed\n");
		HandleError(TRUE);
		PostMessage(hWndMain, WM_DESTROY, EX_OSERR, 0);
		return;
	}

	g_hwndListView = ListViewCreate(hWndMain);
	if (!g_hwndListView) {
		wlog_if_level(wll_fatal, "ListViewCreate failed\n");
		HandleError(TRUE);
		PostMessage(hWndMain, WM_DESTROY, EX_OSERR, 0);
		return;
	}

	g_hwndStatusBar = StatusBarCreate(hWndMain);
	if (!g_hwndStatusBar) {
		wlog_if_level(wll_fatal, "StatusBarCreate failed\n");
		HandleError(TRUE);
		PostMessage(hWndMain, WM_DESTROY, EX_OSERR, 0);
		return;
	}

	BOOL gotSome = WaolawLoad();

	int nCommand = gotSome
		? ID_WAOLA_ADD
		: ID_WAOLA_RESCAN;

	wlog_if_level(wll_info, gotSome
		? "Some hosts are loaded from permanent storage, adding to vault\n"
		: "Nothing has loaded from permanent storage -> starting network discovery\n");

	PostMessage(hWndMain, WM_COMMAND,
		MAKEWPARAM(nCommand, 0), 0);
}

void OnWmNotify(HWND hWndMain, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case ID_LISTVIEW:
		ListViewNotify((LPNMHDR)lParam);
		break;
	case ID_TOOLBAR:
	{
		wtask_t tasks = WaolawGetCurrentTasks();
		int nSelectedHostCount = ListView_GetSelectedCount(g_hwndListView);
		ToolBarNotify((LPNMHDR)lParam, tasks, nSelectedHostCount);
	}
	break;
	default:
	{
		LPNMHDR pNmHdr = (LPNMHDR)lParam;
		UINT_PTR idFor = pNmHdr->idFrom;
		if (idFor >= ID_WAOLA_WAKE_UP
			&& idFor <= ID_EDIT_DELETEHOST) {
			wtask_t tasks = WaolawGetCurrentTasks();
			int nSelectedHostCount = ListView_GetSelectedCount(g_hwndListView);
			ToolBarNotify(pNmHdr, tasks, nSelectedHostCount);
		}
	}
	break;
	}
}

void OnWmSize(HWND hWndMain, WPARAM wParam, LPARAM lParam)
{
	SendMessage(g_hwndToolBar, WM_SIZE, wParam, lParam);
	ListViewResize(hWndMain, g_hwndToolBar, g_hwndStatusBar);
	SendMessage(g_hwndStatusBar, WM_SIZE, wParam, lParam);
}

BOOL OnWmContextMenu(LPARAM lParam)
{
	return ListViewDoContextMenu(lParam);
}

void OnWmClose()
{
	SetCursor(LoadCursor(0, IDC_WAIT));
	WaolawDestroy();
}

void OnWmDestroy(HWND hWnd, WPARAM exitCode)
{
	RECT rcMain;
	GetWindowRect(hWnd, &rcMain);
	SaveWindowPos(&rcMain);

	ListViewDestroy();
	ToolBarDestroy();
	PostQuitMessage((int)exitCode);
}
