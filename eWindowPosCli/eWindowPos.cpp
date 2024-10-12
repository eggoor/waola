/** @file eWindowPos.c
* WaolaG main window implementation.
*/

#include "pch.h"
#include "eWindowPos.h"

using namespace eWindowPosCli;

static BOOL CALLBACK MonitorEnum(HMONITOR, HDC, LPRECT, LPARAM);

bool WindowPos::AdjustWndRect(Rectangle% rcWindow, bool% adjusted)
{
	std::vector<rect_t*> screens;

	EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)&screens);

	return AdjustWndRect(rcWindow, screens, adjusted);
}

bool WindowPos::AdjustWndRect(Rectangle% rcWindow,
	std::vector<rect_t*>& screens, bool% adjusted)
{
	int taskBarHeight = GetTaskBarHeight();

	for (auto& rc : screens) {
		rc->right_bottom.y -= taskBarHeight;
	}

	rect_t rcWnd;
	rcWnd.left_top.x = rcWindow.Left;
	rcWnd.left_top.y = rcWindow.Top;
	rcWnd.right_bottom.x = rcWindow.Right;
	rcWnd.right_bottom.y = rcWindow.Bottom;
	
	BOOL bAdjusted = FALSE;
	bool success = adjust_wnd_rect(&rcWnd, screens.data(),
		static_cast<int>(screens.size()), &bAdjusted);

	adjusted = !!bAdjusted;

	if (success && adjusted) {
		rcWindow.X = rcWnd.left_top.x;
		rcWindow.Y = rcWnd.left_top.y;
		rcWindow.Width = rcWnd.right_bottom.x - rcWnd.left_top.x;
		rcWindow.Height = rcWnd.right_bottom.y - rcWnd.left_top.y;
	}

	for (auto& rc : screens) {
		delete rc;
	}

	return success;
}

int WindowPos::GetTaskBarHeight()
{
	return 32;
}

BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc,
	LPRECT pScreen, LPARAM pData)
{
	auto& vScreens = *(std::vector<rect_t*>*)pData;

	rect_t* rc = new rect_t;

	rc->left_top.x = pScreen->left;
	rc->left_top.y = pScreen->top;
	rc->right_bottom.x = pScreen->right;
	rc->right_bottom.y = pScreen->bottom;

	vScreens.emplace_back(rc);

	return TRUE;
}
