#pragma once

#define TB_SIZE 24

HWND ToolBarCreate(HWND hWndParent);
void ToolBarChangeButtonBitmap(WPARAM commandId, int tbbId);
void ToolBarSetButtonTooltip(LPTOOLTIPTEXT pttText,
	wtask_t tasks, int nSelectedHostCount);
void ToolBarNotify(LPNMHDR, wtask_t, int);
void ToolBarEnableButton(WPARAM commandId, BOOL bEnable);

void ToolBarDestroy();

void ToolBarSetButtonWakeUpTooltip(LPTSTR* pszTooltip, int nSelectedHostCount);
void ToolBarSetButtonRescanTooltip(LPTSTR* pszTooltip, BOOL isDiscovering);
void ToolBarSetButtonRefreshTooltip(LPTSTR* pszTooltip, BOOL isDiscovering);
void ToolBarSetButtonHostAddTooltip(LPTSTR* pszTooltip);
void ToolBarSetButtonHostCopyTooltip(LPTSTR* pszTooltip, int nSelectedHostCount);
void ToolBarSetButtonHostEditTooltip(LPTSTR* pszTooltip, int nSelectedHostCount);
void ToolBarSetButtonHostDeleteTooltip(LPTSTR* pszTooltip, int nSelectedHostCount);
