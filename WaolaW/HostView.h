#pragma once

#include "HostViewData.h"

typedef struct THostView
{
	whost_view_t* hostView;
	LPTSTR* values;
} HostView;

HostView* HostViewCreate(int x, whost_view_t* hw);
void HostViewDestroy(HostView* hostView);

BOOL HostViewImportData(HostView* hostView, const HostViewData* const restrict data);
void HostViewExportData(const HostView* hostView, HostViewData* data);
LPTSTR HostViewGetValue(const HostView* hostView, whost_field_t hf);
LPTSTR HostViewGetExactDisplayName(const HostView* hostView);
BOOL HostViewSetValue(const HostView* hostView,
	LPCTSTR pszValue, whost_field_t hf);
int HostViewCompareValues(const HostView* l,
	const HostView* r, whost_field_t hf);
BOOL HostViewIsFieldUpdated(const HostView* hostView, whost_field_t hf);
whost_view_t* HostViewGetHostView(const HostView* hostView);
int HostViewGetIndex(const HostView* hostView);
void HostViewSetIndex(const HostView* hostView, int x);
LPTSTR HostViewToString(const HostView* hostView);