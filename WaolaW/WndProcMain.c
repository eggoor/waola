#include "pch.h"

#include "WndMain.h"
#include "MessageHandlers.h"

LRESULT CALLBACK MainWndProc(const HWND hWnd, const UINT msg,
	const WPARAM wParam, const LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		OnWmCommand(hWnd, wParam, lParam);
		return 0;
		break;
	case WM_SETCURSOR:
		if (OnWmSetCursor()) {
			return TRUE;
		}
		break;
	case WM_NOTIFY:
		OnWmNotify(hWnd, wParam, lParam);
		return 0;
		break;
	case WM_WAOLA_STATE_CHANGED:
		OnWmWaolaStateChanged((enum wtask)wParam);
		return 0;
		break;
	case WM_WAOLA_HOST_DISCOVERED:
		OnWmWaolaHostDiscovered((waolasc_opcode_t)wParam, (whost_view_t*)lParam);
		return 0;
		break;
	case WM_WAOLA_TASK_FINISHED:
		OnWmWaolaTaskFinished(hWnd, wParam);
		return 0;
		break;
	case WM_CREATE:
		OnWmCreate(hWnd);
		return 0;
		break;
	case WM_SIZE:
		OnWmSize(hWnd, wParam, lParam);
		return 0;
		break;
	case WM_CONTEXTMENU:
		if (OnWmContextMenu(lParam)) {
			return 0;
		}
		break;
	case WM_CLOSE:
		OnWmClose();
		break;
	case WM_DESTROY:
		OnWmDestroy(hWnd, wParam);
		return 0;
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
