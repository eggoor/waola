#include "pch.h"

#include "Resource.h"

#include "StatusBar.h"

extern HINSTANCE g_hInst;

HWND g_hwndStatusBar;

HWND StatusBarCreate(HWND hWndParent)
{
	g_hwndStatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | SBARS_SIZEGRIP,
		0, 0, 0, 0, hWndParent, (HMENU)ID_STATUSBAR, g_hInst, NULL);

	if (!g_hwndStatusBar) {
		return NULL;
	}

	return g_hwndStatusBar;
}

void StatusBarDestroy()
{
	DestroyWindow(g_hwndStatusBar);
}
