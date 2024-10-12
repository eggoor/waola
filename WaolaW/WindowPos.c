#include "pch.h"

#include "WindowPos.h"

static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc,
	LPRECT const pScreen, LPARAM pData);
static int GetTaskBarHeight();

void AdjustWindowPos(HWND hWnd, const RECT* const restrict rcWnd)
{
	HDPA dpaScreens = DPA_Create(2);
	if (!dpaScreens) {
		wlog_if_level(wll_warning, "Unable to make screens dynamic pointer array\n");
		return;
	}

	EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)dpaScreens);

	const int nScreenCount = DPA_GetPtrCount(dpaScreens);
	rect_t** arrScreens = walloc(sizeof(rect_t**) * nScreenCount);
	if (!arrScreens) {
		for (size_t i = nScreenCount; i --> 0;) {
			LPRECT pScreen = DPA_FastGetPtr(dpaScreens, i);
			wfree(pScreen);
		}
		DPA_Destroy(dpaScreens);

		wlog_if_level(wll_warning, "Unable to make screen rects array\n");
		return;
	}

	int nTaskBarHeight = GetTaskBarHeight();

	for (int i = nScreenCount; i --> 0;) {
		arrScreens[i] = DPA_FastGetPtr(dpaScreens, i);
		arrScreens[i]->right_bottom.y -= nTaskBarHeight;
	}

	DPA_Destroy(dpaScreens);

	rect_t rectWnd = {
		.left_top = {
			.x = rcWnd->left,
			.y = rcWnd->top
		},
		.right_bottom = {
			.x = rcWnd->right,
			.y = rcWnd->bottom
		}
	};

	BOOL succeeded = adjust_wnd_rect(&rectWnd, arrScreens, nScreenCount, NULL);

	if (!succeeded) {
		CenterWindow(GetDesktopWindow(), hWnd);
	}
	else {
		SetWindowPos(hWnd, NULL, rectWnd.left_top.x, rectWnd.left_top.y,
			rectWnd.right_bottom.x - rectWnd.left_top.x,
			rectWnd.right_bottom.y - rectWnd.left_top.y, 0);
	}

	for (size_t i = nScreenCount; i --> 0;) {
		wfree(arrScreens[i]);
	}

	wfree(arrScreens);
}

void CenterWindow(HWND hwndSource, HWND hwndTarget)
{
	RECT rcMain;
	GetWindowRect(hwndSource, &rcMain);

	RECT rcTarget;
	GetWindowRect(hwndTarget, &rcTarget);

	int widthMain = rcMain.right - rcMain.left;
	int heightMain = rcMain.bottom - rcMain.top;

	POINT ptMainCenter = {
		.x = rcMain.left + widthMain / 2 + widthMain % 2,
		.y = rcMain.top + heightMain / 2 + heightMain % 2
	};

	int widthTarget = rcTarget.right - rcTarget.left;
	int heightTarget = rcTarget.bottom - rcTarget.top;

	POINT ptTargetCenter = {
		.x = rcTarget.left + widthTarget / 2 + widthTarget % 2,
		.y = rcTarget.top + heightTarget / 2 + heightTarget % 2
	};

	POINT ptDiff = {
		.x = ptMainCenter.x - ptTargetCenter.x,
		.y = ptMainCenter.y - ptTargetCenter.y
	};

	rcTarget.left += ptDiff.x;
	rcTarget.top += ptDiff.y;
	rcTarget.right += ptDiff.x;
	rcTarget.bottom += ptDiff.y;

	SetWindowPos(hwndTarget, hwndSource, rcTarget.left, rcTarget.top,
		rcTarget.right - rcTarget.left, rcTarget.bottom - rcTarget.top, 0);
}

static int GetTaskBarHeight()
{
	return 32;
}

static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc,
	LPRECT const pScreen, LPARAM pData)
{
	HDPA mc = (HDPA)pData;

	rect_t* rcScreen = walloc(sizeof(rect_t));

	if (!rcScreen) {
		wlog_if_level(wll_warning, "No %zuB of free memory for RECT\n",
			sizeof(rect_t));
		goto end;
	}

	rcScreen->left_top.x = pScreen->left;
	rcScreen->left_top.y = pScreen->top;
	rcScreen->right_bottom.x = pScreen->right;
	rcScreen->right_bottom.y = pScreen->bottom;

	DPA_AppendPtr(mc, rcScreen);

end:
	return TRUE;
}
