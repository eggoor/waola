#include "pch.h"

#include "Resource.h"

#include "ToolBarButtons.h"
#include "WmCommandRedirectWndProc.h"

#include "ToolBar.h"

extern HINSTANCE g_hInst;

HWND g_hwndToolBar;

static HBITMAP g_hbmpTooBbar;
static HIMAGELIST g_hImageList;

HWND ToolBarCreate(HWND hWndParent)
{
	g_hwndToolBar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | TBSTYLE_TOOLTIPS,
		0, 0, 0, 0, hWndParent, (HMENU)ID_TOOLBAR, g_hInst, NULL);

	if (!g_hwndToolBar) {
		return NULL;
	}

	g_hImageList = ImageList_Create(TB_SIZE, TB_SIZE,
		ILC_COLOR32 | ILC_MASK, 1, 0);

	if (!g_hImageList) {
		return NULL;
	}

	g_hbmpTooBbar = (HBITMAP)LoadImage(g_hInst, MAKEINTRESOURCE(IDB_TOOLBAR),
		IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);

	if (!g_hImageList) {
		return NULL;
	}

	ImageList_Add(g_hImageList, g_hbmpTooBbar, NULL);

	SendMessage(g_hwndToolBar, TB_SETIMAGELIST, 0, (LPARAM)g_hImageList);
	SendMessage(g_hwndToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(g_hwndToolBar, TB_ADDBUTTONS, (WPARAM)_countof(g_ToolBarButtons), (LPARAM)&g_ToolBarButtons);
	SendMessage(g_hwndToolBar, TB_AUTOSIZE, 0, 0);

	SetWindowLongPtr(g_hwndToolBar, GWLP_USERDATA,
		(LONG_PTR)SetWindowLongPtr(g_hwndToolBar,
			GWLP_WNDPROC, (LONG_PTR)WmCommandRedirectWndProc));


	for (int i = _countof(g_ToolBarButtons); i --> 0;) {
		for (int j = 2; j --> 0;) {
			LoadString(g_hInst, g_ToolBarToolTipStringIdMap[i][j],
				g_ToolBarButtonToolTips[i][j], MAX_TOOLTIP);
		}
	}

	return g_hwndToolBar;
}

void ToolBarSetButtonTooltip(LPTOOLTIPTEXT pttText,
	wtask_t tasks, int nSelectedHostCount)
{
	switch (pttText->hdr.idFrom)
	{
	case ID_WAOLA_WAKE_UP:
	{
		ToolBarSetButtonWakeUpTooltip(&pttText->lpszText,
			nSelectedHostCount);
	}
	break;
	case ID_WAOLA_RESCAN:
	{
		ToolBarSetButtonRescanTooltip(&pttText->lpszText,
			tasks & wt_discover);
	}
	break;
	case ID_WAOLA_REFRESH:
	{
		ToolBarSetButtonRefreshTooltip(&pttText->lpszText,
			tasks & wt_refresh);
	}
	break;
	case ID_EDIT_ADDHOST:
	{
		ToolBarSetButtonHostAddTooltip(&pttText->lpszText);
	}
	break;
	case ID_EDIT_EDITHOST:
	{
		ToolBarSetButtonHostEditTooltip(&pttText->lpszText,
			nSelectedHostCount);
	}
	break;
	case ID_EDIT_COPYHOST:
	{
		ToolBarSetButtonHostCopyTooltip(&pttText->lpszText,
			nSelectedHostCount);
	}
	break;
	}
}

void ToolBarNotify(LPNMHDR pNmHdr, wtask_t tasks,
	int nSelectedHostCount)
{
	switch (pNmHdr->code)
	{
	case TTN_NEEDTEXTW:
		ToolBarSetButtonTooltip((LPTOOLTIPTEXT)pNmHdr,
			tasks, nSelectedHostCount);
	}
}

void ToolBarChangeButtonBitmap(WPARAM commandId, int tbbId)
{
	SendMessage(g_hwndToolBar, TB_CHANGEBITMAP,
		commandId, MAKELPARAM(tbbId, 0));
}

void ToolBarEnableButton(WPARAM commandId, BOOL bEnable)
{
	SendMessage(g_hwndToolBar, TB_ENABLEBUTTON,
		commandId, MAKELPARAM(bEnable, 0));
}

void ToolBarDestroy()
{
	ImageList_Destroy(g_hImageList);
	DeleteObject(g_hbmpTooBbar);
	DestroyWindow(g_hwndToolBar);
}

void ToolBarSetButtonWakeUpTooltip(LPTSTR* ppszTooltip,
	int nSelectedHostCount)
{
	*ppszTooltip = g_ToolBarButtonToolTips[0][nSelectedHostCount > 0 ? 0 : 1];
}

void ToolBarSetButtonRescanTooltip(LPTSTR* ppszTooltip,
	BOOL isDiscovering)
{
	*ppszTooltip = g_ToolBarButtonToolTips[2][isDiscovering ? 1 : 0];
}

void ToolBarSetButtonRefreshTooltip(LPTSTR* ppszTooltip,
	BOOL isDiscovering)
{
	*ppszTooltip = g_ToolBarButtonToolTips[3][isDiscovering ? 1 : 0];
}

void ToolBarSetButtonHostAddTooltip(LPTSTR* ppszTooltip)
{
	*ppszTooltip = g_ToolBarButtonToolTips[5][0];
}

void ToolBarSetButtonHostCopyTooltip(LPTSTR* ppszTooltip,
	int nSelectedHostCount)
{
	*ppszTooltip = g_ToolBarButtonToolTips[6][nSelectedHostCount > 0 ? 0 : 1];
}

void ToolBarSetButtonHostEditTooltip(LPTSTR* ppszTooltip,
	int nSelectedHostCount)
{
	*ppszTooltip = g_ToolBarButtonToolTips[7][nSelectedHostCount == 1 ? 0 : 1];
}

void ToolBarSetButtonHostDeleteTooltip(LPTSTR* ppszTooltip,
	int nSelectedHostCount)
{
	*ppszTooltip = g_ToolBarButtonToolTips[8][nSelectedHostCount > 0 ? 0 : 1];
}
