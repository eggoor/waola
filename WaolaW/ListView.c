#include "pch.h"

#include "Resource.h"
#include "ToolBar.h"
#include "CommandHandlers.h"
#include "WmCommandRedirectWndProc.h"

#include "ListViewPrivate.h"

extern HINSTANCE g_hInst;

HWND g_hwndListView;

static HMENU g_hContextMenu;
static HBITMAP g_hbmpArrowUp;
static HBITMAP g_hbmpArrowDown;
static BOOL g_bSortAsc = TRUE;
static int g_nSortColumn = -1;

static TCHAR g_ColumnCaptions[hwf_count][DEF_STR_BUFFER_SIZE] = {
	TEXT("Friendly name")
	, TEXT("Hostname")
	, TEXT("IP addr")
	, TEXT("MAC addr")
	, TEXT("Last seen online")
	, TEXT("Result")
};

static int g_ColumnCaptionsStringIdMap[hwf_count] = {
	IDS_FRIENDLY_NAME
	, IDS_HOST_NAME
	, IDS_IP_ADDRESS
	, IDS_MAC_ADDRESS
	, IDS_LAST_SEEN_ONLINE
	, IDS_OP_RESULT
};

HWND ListViewCreate(HWND hwndParent)
{
	DWORD dwStyle = WS_TABSTOP | WS_CHILD | WS_VISIBLE
		| LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS;

	g_hwndListView = CreateWindowEx(WS_EX_CLIENTEDGE,
		WC_LISTVIEW, _T(""), dwStyle, 0, 0, 0, 0,
		hwndParent, (HMENU)ID_LISTVIEW, g_hInst, NULL);

	if (!g_hwndListView) {
		wlog_if_level(wll_error, "CreateWindowEx(WC_LISTVIEW) failed\n");
		goto end;
	}

	ListView_SetExtendedListViewStyle(g_hwndListView,
		LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	SetWindowLongPtr(g_hwndListView, GWLP_USERDATA,
		(LONG_PTR)SetWindowLongPtr(g_hwndListView,
			GWLP_WNDPROC, (LONG_PTR)WmCommandRedirectWndProc));

	g_hContextMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDM_CONTEXT_MENU));
	g_hbmpArrowUp = (HBITMAP)LoadImage(g_hInst, MAKEINTRESOURCE(IDB_ARROW_UP),
		IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
	g_hbmpArrowDown = (HBITMAP)LoadImage(g_hInst, MAKEINTRESOURCE(IDB_ARROW_DOWN),
		IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);

	for (int i = hwf_count; i --> 0;) {
		LoadString(g_hInst, g_ColumnCaptionsStringIdMap[i],
			g_ColumnCaptions[i], DEF_STR_BUFFER_SIZE);
	}

	Init();

end:
	return g_hwndListView;
}

void ListViewResize(HWND hwndParent, HWND hwndToolBar, HWND hwndStatusBar)
{
	RECT rcParent;
	GetClientRect(hwndParent, &rcParent);

	RECT rcToolBar;
	GetClientRect(hwndToolBar, &rcToolBar);

	RECT rcStatusBar;
	GetClientRect(hwndStatusBar, &rcStatusBar);

	rcParent.left += 1;
	rcParent.top += rcToolBar.bottom - rcToolBar.top + 1;
	rcParent.right -= 1;
	rcParent.bottom -= rcStatusBar.bottom - rcStatusBar.top;

	MoveWindow(g_hwndListView, rcParent.left, rcParent.top,
		rcParent.right - rcParent.left, rcParent.bottom - rcParent.top, TRUE);
}

void ListViewAddHost(const HostView* hostView)
{
	int x = HostViewGetIndex(hostView);

	LV_ITEM lvi = {
		.mask = LVIF_PARAM | LVIF_NORECOMPUTE,
		.iItem = x,
		.lParam = (LPARAM)hostView
	};

	ListView_InsertItem(g_hwndListView, &lvi);

	lvi.mask |= LVIF_TEXT;
	lvi.mask &= ~LVIF_PARAM;

	for (whost_field_t hf = hwf_count; hf --> 0;) {
		LPTSTR pszValue = HostViewGetValue(hostView, hf);

		lvi.iSubItem = hf;
		lvi.pszText = pszValue;
		lvi.cchTextMax = pszValue ? (int)_tcslen(pszValue) + 1 : 0;
		SNDMSG(g_hwndListView, LVM_SETITEMTEXT, x, (LPARAM)&lvi);
	}

	if (-1 != g_nSortColumn) {
		g_nSortColumn = -1;
		SetSortIcon(g_nSortColumn, FALSE);
	}
}

void ListViewUpdateHost(const HostView* hostView)
{
	LVFINDINFO lvf = {
		.flags = LVFI_PARAM,
		.lParam = (LPARAM)hostView
	};

	int x = ListView_FindItem(g_hwndListView, -1, &lvf);

	for (whost_field_t hf = hwf_count; hf --> 0;) {
		if (HostViewIsFieldUpdated(hostView, hf)) {
			LPTSTR value = HostViewGetValue(hostView, hf);
			ListView_SetItemText(g_hwndListView, x, hf, value);
		}
	}
}

void ListViewDeleteHosts(HostView** hostViews, uint32_t nCount)
{
	for (int i = nCount; i --> 0;) {
		DeleteHost(hostViews[i]);
	}
}

HostView* ListViewGetSelectedHost()
{
	HostView* hostView = NULL;

	 int x = ListView_GetNextItem(g_hwndListView,
		-1, LVNI_SELECTED);
	 
	 if (-1 != x) {
		 LVITEM lvi = {
	 .mask = LVIF_PARAM,
	 .iItem = x
		 };

		 ListView_GetItem(g_hwndListView, &lvi);
		 hostView = (HostView*)lvi.lParam;
	 }

	 return hostView;
}

HostView** ListViewGetSelectedHosts(uint32_t* pnCount)
{
	HostView** hostViews = NULL;
	*pnCount = ListView_GetSelectedCount(g_hwndListView);
	hostViews = walloc(*pnCount * sizeof(HostView*));	

	int x = -1;

	for (int i = *pnCount; i --> 0;) {
		x = ListView_GetNextItem(g_hwndListView,
			x, LVNI_SELECTED);
		
		LVITEM lvi = {
			.mask = LVIF_PARAM,
			.iItem = x
		};

		ListView_GetItem(g_hwndListView, &lvi);
		hostViews[i] = (HostView*)lvi.lParam;
	}

	return hostViews;
}

void ListViewNotify(LPNMHDR pNmHdr)
{
	switch (pNmHdr->code)
	{
	case LVN_COLUMNCLICK:
		OnColumnClick((NMLISTVIEW*)pNmHdr);
		break;
	case LVN_ITEMCHANGED:
		OnItemChanged((NMLISTVIEW*)pNmHdr);
		break;
	case NM_DBLCLK:
		OnItemDoucleclicked((LPNMITEMACTIVATE)pNmHdr);
		break;
	}
}

BOOL ListViewDoContextMenu(LPARAM lParam)
{
	HMENU hSubMenu = GetSubMenu(g_hContextMenu, 0);

	return TrackPopupMenu(hSubMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		LOWORD(lParam), HIWORD(lParam), 0, g_hwndListView, NULL);
}

int ListViewGetSelectedCount()
{
	return ListView_GetSelectedCount(g_hwndListView);
}

void ListViewSelectAllHosts()
{
	for (int i = ListView_GetItemCount(g_hwndListView); i --> 0;) {
		ListView_SetItemState(g_hwndListView, i,
			LVIS_SELECTED, LVIS_SELECTED);
	}
}

void ListViewDestroy()
{
	DestroyMenu(g_hContextMenu);

	DeleteObject(g_hbmpArrowUp);
	DeleteObject(g_hbmpArrowDown);

	DestroyWindow(g_hwndListView);
}

static void Init()
{
	ListView_DeleteAllItems(g_hwndListView);

	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 128;

	for (int i = 0; i < hwf_count; i++)
	{
		lvColumn.pszText = g_ColumnCaptions[i];
		ListView_InsertColumn(g_hwndListView, i, &lvColumn);
	}
}

static void PositionHeader()
{
	HWND hwndHeader = GetWindow(g_hwndListView, GW_CHILD);
	DWORD dwStyle = GetWindowLong(g_hwndListView, GWL_STYLE);

	dwStyle |= LVS_NOSCROLL;
	SetWindowLong(g_hwndListView, GWL_STYLE, dwStyle);

	if (((dwStyle & LVS_TYPEMASK) == LVS_REPORT) && hwndHeader)
	{
		RECT rc;
		GetClientRect(g_hwndListView, &rc);

		WINDOWPOS wpos;

		HD_LAYOUT hdLayout = {
			.prc = &rc,
			.pwpos = &wpos
		};

		Header_Layout(hwndHeader, &hdLayout);

		SetWindowPos(hwndHeader, wpos.hwndInsertAfter, wpos.x,
			wpos.y, wpos.cx, wpos.cy, wpos.flags | SWP_SHOWWINDOW);

		ListView_EnsureVisible(g_hwndListView, 0, FALSE);
	}
}

static void DeleteHost(HostView* hostView)
{
	LVFINDINFO lvf = {
		.flags = LVFI_PARAM,
		.lParam = (LPARAM)hostView
	};

	int x = ListView_FindItem(g_hwndListView, -1, &lvf);

	ListView_DeleteItem(g_hwndListView, x);
}

static void OnColumnClick(NMLISTVIEW* pNmListView)
{
	if (g_nSortColumn == pNmListView->iSubItem) {
		g_bSortAsc = !g_bSortAsc;
	}
	else {
		g_nSortColumn = pNmListView->iSubItem;
		g_bSortAsc = TRUE;
	}

	LPARAM lParamSort = g_nSortColumn + 1;

	if (!g_bSortAsc) {
		lParamSort = -lParamSort;
	}

	SendMessage(g_hwndListView, WM_SETREDRAW, FALSE, 0);

	ListView_SortItemsEx(pNmListView->hdr.hwndFrom,
		CompareProc, lParamSort);
	SetSortIcon(g_nSortColumn, g_bSortAsc);

	SendMessage(g_hwndListView, WM_SETREDRAW, TRUE, 0);
}

static void OnItemChanged(NMLISTVIEW* pNmListView)
{
	UpdateUI();
}

static void OnItemDoucleclicked(LPNMITEMACTIVATE pNmActivate)
{
	OnCommandWakeUpSelected();
}

static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	BOOL bSortAsc = (lParamSort > 0);
	int xCol = abs((int)lParamSort) - 1;

	LVITEM lvi = {
		.mask = LVIF_PARAM,
		.iItem = (int)lParam1
	};

	ListView_GetItem(g_hwndListView, &lvi);
	HostView* hostViewL = (HostView*)lvi.lParam;

	lvi.iItem = (int)lParam2;
	ListView_GetItem(g_hwndListView, &lvi);
	HostView* hostViewR = (HostView*)lvi.lParam;

	if (!bSortAsc) {
		swap(&hostViewL, &hostViewR);
	}

	return HostViewCompareValues(hostViewL, hostViewR, xCol);
}

static void SetSortIcon(const int xCol, int isAscending)
{
	HWND header = ListView_GetHeader(g_hwndListView);

	for (int i = hwf_count; i --> 0;)
	{
		HDITEM hdi = { .mask = HDI_FORMAT | HDI_BITMAP };
		Header_GetItem(header, i, &hdi);

		if (xCol == i) {
			hdi.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT;
			hdi.hbm = isAscending ? g_hbmpArrowUp : g_hbmpArrowDown;
		}
		else {
			hdi.mask &= ~HDI_BITMAP;
			hdi.fmt &= ~(HDF_BITMAP | HDF_BITMAP_ON_RIGHT);
		}

		Header_SetItem(header, i, &hdi);
	}
}
