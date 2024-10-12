#include "pch.h"

#include "HostView.h"

LPTSTR HostViewCreateValue(const HostView* hostView, whost_field_t hf);
void HostViewFreeValue(const HostView* hostView, whost_field_t hf);

void HostViewFreeValues(LPTSTR* values);

HostView* HostViewCreate(int x, whost_view_t* hw)
{
	HostView* hostView = walloc(sizeof(struct THostView));
	if (!hostView) {
		wlog_if_level(wll_warning, "No %zuB of free memory for host view\n",
			sizeof(struct THostView));
		goto end;
	}

	hostView->hostView = hw;
	HostViewSetIndex(hostView, x);
	size_t valuesSize = sizeof(TCHAR*) * hwf_count;
	hostView->values = walloc(valuesSize);
	memset(hostView->values, 0, valuesSize);

end:
	return hostView;
}

void HostViewDestroy(HostView* hostView)
{
	HostViewFreeValues(hostView->values);
	wfree(hostView);
}

void HostViewFreeValues(LPTSTR* values)
{
#ifdef UNICODE
	for (int i = hwf_count; i --> 0;) {
		wfree(values[i]);
	}
#endif

	wfree(values);
}

LPTSTR HostViewGetValue(const HostView* hostView, whost_field_t hf)
{
	if (whost_view_is_field_updated(hostView->hostView, hf)) {
		HostViewFreeValue(hostView, hf);
	}

	if (!hostView->values[hf]) {
		hostView->values[hf] = HostViewCreateValue(hostView, hf);
		if (whost_view_is_field_updated(hostView->hostView, hf)) {
			whost_view_set_field_change_processed(hostView->hostView,
				whost_field_2_changed_field(hf));
		}
	}

	return hostView->values[hf];
}

LPTSTR HostViewGetExactDisplayName(const HostView* hostView)
{
	return
#ifdef UNICODE
	wstr_8_to_16(whost_view_get_exact_display_name(hostView->hostView))
#else
	StrDup(whost_view_get_exact_display_name(hostView->hostView))
#endif
	;
}

BOOL HostViewSetValue(const HostView* hostView,
	LPCTSTR pszValue, whost_field_t hf)
{
	BOOL isChanged = FALSE;
	char* value;

#ifdef UNICODE
	value = wstr_16_to_8(pszValue);
#else
	value = pszValue;
#endif

	switch (hf)
	{
	case hwf_display_name:
	{
		isChanged = whost_view_set_display_name(hostView->hostView, value);
	}
	break;
	case hwf_host_name:
	{
		isChanged = whost_view_set_hostname(hostView->hostView, value);
	}
	break;
	case hwf_ip_address:
	{
		isChanged = whost_view_set_ip_addr(hostView->hostView, value);
	}
	break;
	case hwf_mac_address:
	{
		isChanged = whost_view_set_mac_addr(hostView->hostView, value);
	}
	break;
	default:
		wlog_if_level(wll_warning, "Unknown host field %d in HostViewSetValue\n", hf);
	}

#ifdef UNICODE
	wfree(value);
#endif

	return isChanged;
}

LPTSTR HostViewCreateValue(const HostView* hostView, whost_field_t hf)
{
	LPTSTR pszDstValue = NULL;
	const char* pszSrcValue;

	switch (hf)
	{
	case hwf_display_name:
	{
		pszSrcValue = whost_view_get_display_name(hostView->hostView);
	}
	break;
	case hwf_host_name:
	{
		pszSrcValue = whost_view_get_hostname(hostView->hostView);
	}
	break;
	case hwf_ip_address:
	{
		pszSrcValue = whost_view_get_ip_addr(hostView->hostView);
	}
	break;
	case hwf_mac_address:
	{
		pszSrcValue = whost_view_get_mac_addr(hostView->hostView);
	}
	break;
	case hwf_last_seen_online:
	{
		pszSrcValue = whost_view_get_last_seen_online(hostView->hostView);
	}
	break;
	case hwf_op_result:
	{
		pszSrcValue = whost_view_get_op_result(hostView->hostView);
	}
	break;
	default:
	{
		wlog_if_level(wll_warning, "Unknown host field %d in HostViewCreateValue\n", hf);
		pszSrcValue = NULL;
	}
	}

#ifdef UNICODE
	pszDstValue = wstr_8_to_16(pszSrcValue);
#else
	pszDstValue = pszSrcValue;
#endif

	return pszDstValue;
}

void HostViewFreeValue(const HostView* hostView, whost_field_t hf)
{
#ifdef UNICODE
	if (hostView->values[hf]) {
		wfree(hostView->values[hf]);
		hostView->values[hf] = NULL;
	}
#endif
}

int HostViewCompareValues(const HostView* l,
	const HostView* r, whost_field_t hf)
{
	int res = 0;

	whost_view_t* hwL = l->hostView;
	whost_view_t* hwR = r->hostView;

	switch (hf) {
	case hwf_display_name:
	{
		res = whost_view_compare_display_name(hwL, hwR);
	}
	case hwf_host_name:
	{
		res = whost_view_compare_host_name(hwL, hwR);
	}
	break;
	case hwf_ip_address:
	{
		res = whost_view_compare_ip_addr(hwL, hwR);
	}
	break;
	case hwf_mac_address:
	{
		res = whost_view_compare_mac_addr(hwL, hwR);
	}
	break;
	case hwf_last_seen_online:
	{
		res = whost_view_compare_last_seen_online(hwL, hwR);
	}
	break;
	case hwf_op_result:
	{
		res = 0;
	}
	break;
	default:
	{
		wlog_if_level(wll_warning, "Unknown host field %d in HostViewCompareValues\n", hf);
	}
	}

	return res;
}

BOOL HostViewImportData(HostView* hostView, const HostViewData* const restrict data)
{
	BOOL isChanged = FALSE;
	isChanged |= HostViewSetValue(hostView, data->pszDisplayName, hwf_display_name);
	isChanged |= HostViewSetValue(hostView, data->pszHostname, hwf_host_name);
	isChanged |= HostViewSetValue(hostView, data->pszIpAddr, hwf_ip_address);
	isChanged |= HostViewSetValue(hostView, data->pszMacAddr, hwf_mac_address);

	return isChanged;
}

void HostViewExportData(const HostView* hostView, HostViewData* data)
{
	data->pszDisplayName = HostViewGetExactDisplayName(hostView);
	data->pszHostname = StrDup(HostViewGetValue(hostView, hwf_host_name));
	data->pszIpAddr = StrDup(HostViewGetValue(hostView, hwf_ip_address));
	data->pszMacAddr = StrDup(HostViewGetValue(hostView, hwf_mac_address));
}

BOOL HostViewIsFieldUpdated(const HostView* hostView, whost_field_t hf)
{
	return whost_view_is_field_updated(hostView->hostView, hf);
}

whost_view_t* HostViewGetHostView(const HostView* hostView)
{
	return hostView->hostView;
}

int HostViewGetIndex(const HostView* hostView)
{
	const whost_view_t* hw = HostViewGetHostView(hostView);
	const int* pX = whost_view_get_extra_data(hw);

	return pX ? *pX : -1;
}

void HostViewSetIndex(const HostView* hostView, int x)
{
	whost_view_t* hw = HostViewGetHostView(hostView);
	whost_view_set_extra_data(hw, &x, sizeof(x));
}

LPTSTR HostViewToString(const HostView* hostView)
{
	LPTSTR pszHostViewString = NULL;
	char* whost_view_string = whost_view_to_string(hostView->hostView);

#ifdef UNICODE
	pszHostViewString = wstr_8_to_16(whost_view_string);
	wfree(whost_view_string);
#else
	pszHostViewString = whost_view_string;
#endif
	return pszHostViewString;
}