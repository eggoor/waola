#pragma once

#include "HostView.h"

HWND ListViewCreate(HWND);
void ListViewResize(HWND, HWND, HWND);
void ListViewAddHost(const HostView* hostView);
void ListViewUpdateHost(const HostView* hostView);
void ListViewNotify(LPNMHDR pNmHdr);
BOOL ListViewDoContextMenu(LPARAM);
HostView* ListViewGetSelectedHost();
HostView** ListViewGetSelectedHosts(uint32_t* pnCount);
void ListViewDeleteHosts(HostView** hostViews, uint32_t nCount);
int ListViewGetSelectedCount();
void ListViewSelectAllHosts();

void ListViewDestroy();
