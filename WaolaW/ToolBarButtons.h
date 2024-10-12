#pragma once

static TBBUTTON g_ToolBarButtons[] =
{
	{ 0, ID_WAOLA_WAKE_UP,		0,					TBSTYLE_BUTTON,	0, 0},
	{ 0, 0,						0,					TBSTYLE_SEP,	0, 0},
	{ 1, ID_WAOLA_RESCAN,		TBSTATE_ENABLED,	TBSTYLE_BUTTON,	0, 0 },
	{ 3, ID_WAOLA_REFRESH,		TBSTATE_ENABLED,	TBSTYLE_BUTTON,	0, 0 },
	{ 0, 0,						0,					TBSTYLE_SEP,	0, 0},
	{ 5, ID_EDIT_COPYHOST,		0,					TBSTYLE_BUTTON,	0, 0},
	{ 0, 0,						0,					TBSTYLE_SEP,	0, 0},
	{ 6, ID_EDIT_ADDHOST,		TBSTATE_ENABLED,	TBSTYLE_BUTTON,	0, 0},
	{ 7, ID_EDIT_EDITHOST,		0,					TBSTYLE_BUTTON,	0, 0},
	{ 8, ID_EDIT_DELETEHOST,	0,					TBSTYLE_BUTTON,	0, 0},
	{ 0, 0,						0,					TBSTYLE_SEP,	0, 0},
	{ 9, IDM_ABOUT,				TBSTATE_ENABLED,	TBSTYLE_BUTTON,	0, 0},
};

#define MAX_TOOLTIP (DEF_STR_BUFFER_SIZE + DEF_STR_BUFFER_SIZE / 2)

static TCHAR g_ToolBarButtonToolTips[_countof(g_ToolBarButtons)]
[2][MAX_TOOLTIP] = {
	{ TEXT("Click to wake up selected host(s)"),	TEXT("Select one or more hosts to enable") }
	, { _T(""),										_T("")} // separator
	, { TEXT("Click to rescan network"),			TEXT("Click to stop scan") }
	, { TEXT("Click to refresh hosts"),				TEXT("Click to stop refreshing") }
	, { _T(""),										_T("")} // separator
	, { TEXT("Copy host"),							TEXT("Select one or more hosts to enable") }
	, { _T(""),										_T("")} // separator
	, { TEXT("Add new host"),						_T("")}
	, { TEXT("Edit host"),							TEXT("Select a host to enable") }
	, { TEXT("Delete host"),						TEXT("Select one or more hosts to enable") }
	, { _T(""),										_T("")} // separator
	, { TEXT("About Waola"),						TEXT("") }
};

static int g_ToolBarToolTipStringIdMap[_countof(g_ToolBarButtons)][2] = {
	{ IDS_TT_WAKE_UP,		IDS_TT_SELECT_HOSTS }
	, { 0,					0 }
	, { IDS_TT_RESCAN,		IDS_TT_STOP_SCAN}
	, { IDS_TT_REFRESH,		IDS_TT_STOP_REFRESH}
	, { 0,					0 }
	, { IDS_TT_HOST_COPY,	IDS_TT_SELECT_HOSTS }
	, { 0,					0 }
	, { IDS_TT_HOST_ADD,	0 }
	, { IDS_TT_HOST_EDIT,	IDS_TT_SELECT_HOST }
	, { IDS_TT_HOST_DELETE,	IDS_TT_SELECT_HOSTS }
	, { 0,					0 }
	, { IDS_TT_ABOUT,		IDS_TT_ABOUT }
};
