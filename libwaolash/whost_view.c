#include "pch.h"

#include "include/libwaolash.h"

#include "whost_view.h"

static const char* g_sNever = "Never";
static const char* g_sSuccess = "Success";
static const char* g_sFail = "Fail";

whost_view_t* whost_view_make_impl(whost_t* const restrict host)
{
	whost_view_t* hw = walloc_s(sizeof(struct whost_view));

	if (!hw) {
		goto end;
	}

	if (!host) {
		wlog_if_level(wll_error, "Null host provided\n");
	}

	hw->host = host;
	hw->ipAddrString = NULL;
	hw->macAddrString = NULL;
	hw->lastSeenOnlineString = NULL;
	hw->extraData = NULL;

end:
	return hw;
}

void whost_view_free(whost_view_t* const restrict hw)
{
	wfree_s(hw->extraData);
	wfree_s(hw->ipAddrString);
	wfree_s(hw->macAddrString);
	wfree_s(hw->lastSeenOnlineString);
	whost_free(hw->host);
	wfree_s(hw);
}

const char* whost_view_get_display_name_s(const whost_view_t* const restrict hw)
{
	const char* displayName = whost_get_display_name_s(hw->host);

	if (wstr_is_empty_s(displayName)) {
		displayName = whost_view_get_hostname_s(hw);
		if (wstr_is_empty_s(displayName)) {
			displayName = whost_view_get_ip_addr_s(hw);
			if (wstr_is_empty_s(displayName)) {
				displayName = whost_view_get_mac_addr_s(hw);
			}
		}
	}

	return displayName;
}

const char* whost_view_get_exact_display_name_s(const whost_view_t* const restrict hw)
{
	return whost_get_display_name_s(hw->host);
}

const char* whost_view_get_hostname_s(const whost_view_t* const restrict hw)
{
	return whost_get_hostname_s(hw->host);
}

const char* whost_view_get_ip_addr_s(const whost_view_t* const restrict hw)
{
	if (!hw->ipAddrString ||
		whost_is_field_updated_s(hw->host, hwf_ip_address)) {
		update_whost_view_ip_addr((whost_view_t*)hw);
	}

	return hw->ipAddrString;
}

const char* whost_view_get_mac_addr_s(const whost_view_t* const restrict hw)
{
	if (!hw->macAddrString ||
		whost_is_field_updated_s(hw->host, hwf_mac_address)) {
		update_whost_view_mac_addr((whost_view_t*)hw);
	}

	return hw->macAddrString;
}

const char* whost_view_get_last_seen_online_s(const whost_view_t* const restrict hw)
{
	time_t lso = whost_get_last_seen_online_s(hw->host);

	if (!lso) {
		return g_sNever;
	}

	if (!hw->lastSeenOnlineString ||
		whost_is_field_updated_s(hw->host, hwf_last_seen_online)) {
		update_whost_view_last_seen_online((whost_view_t*)hw, lso);
	}

	return hw->lastSeenOnlineString;
}

const char* whost_view_get_op_result_s(const whost_view_t* const restrict hw)
{
	const char* ors = NULL;
	whost_op_result_t or = whost_get_op_result_s(hw->host);

	switch (or )
	{
	case wor_undefined:
		// Do nothing
		break;
	case wor_success:
		ors = g_sSuccess;
		break;
	case wor_fail:
		ors = g_sFail;
		break;
	default:
		wlog_if_level(wll_warning, "Invalid operation result %d got from whost_get_op_result_s in %s\n",
			or , __func__);
	}

	return ors;
}

const whost_t* whost_view_get_host_s(const whost_view_t* const restrict hw)
{
	return hw->host;
}

whost_changed_field_t whost_view_get_state_s(const whost_view_t* const restrict hw)
{
	return whost_get_state_s(hw->host);
}

BOOL whost_view_is_updated_s(const whost_view_t* const restrict hw)
{
	return whost_view_get_state_s(hw) != cf_unchanged;
}

void* whost_view_get_extra_data_s(const whost_view_t* const restrict hw)
{
	return hw->extraData;
}

const char* whost_view_get_host_id_s(const whost_view_t* const restrict hw)
{
	const char* id = NULL;

	if (!wstr_is_empty_s(id = whost_view_get_display_name_s(hw))) {
		return id;
	}

	if (!wstr_is_empty_s(id = whost_view_get_hostname_s(hw))) {
		return id;
	}

	if (!wstr_is_empty_s(id = whost_view_get_ip_addr_s(hw))) {
		return id;
	}

	return whost_view_get_mac_addr_s(hw);
}

BOOL whost_view_is_field_updated_s(const whost_view_t* hw,
	whost_field_t hf)
{
	return whost_is_field_updated_s(hw->host, hf);
}

char* whost_view_to_string_s(const whost_view_t* const restrict hw)
{
	char* str = NULL;
	const char* val = whost_view_get_mac_addr_s(hw);

	if (val) {
		str = wstrdup_s(val);
		if (!str) {
			goto end;
		}
	}

	str = wstrcat_s(str, "\t");
	if (!str) {
		goto end;
	}

	val = whost_view_get_ip_addr_s(hw);

	if (val) {
		str = wstrcat_s(str, val);
		if (!str) {
			goto end;
		}
	}

	str = wstrcat_s(str, "\t");
	if (!str) {
		goto end;
	}

	val = whost_view_get_hostname_s(hw);

	if (val) {
		str = wstrcat_s(str, val);
		if (!str) {
			goto end;
		}
	}

	str = wstrcat_s(str, "\t");
	if (!str) {
		goto end;
	}

	val = whost_view_get_exact_display_name_s(hw);

	if (val) {
		str = wstrcat_s(str, val);
	}

end:
	return str;
}

int whost_view_compare_s(const whost_view_t* l,
	const whost_view_t* r)
{
	const whost_t* left_host = whost_view_get_host_s(l);
	const whost_t* right_host = whost_view_get_host_s(r);

	return whost_compare_s(left_host, right_host);
}

int whost_view_compare_display_name_s(const whost_view_t* l,
	const whost_view_t* r)
{
	const char* sl = whost_view_get_display_name_s(l);
	const char* sr = whost_view_get_display_name_s(r);

	return wstrcmp_s(sl, sr);
}

int whost_view_compare_host_name_s(const whost_view_t* l,
	const whost_view_t* r)
{
	const char* sl = whost_view_get_hostname_s(l);
	const char* sr = whost_view_get_hostname_s(r);

	return wstrcmp_s(sl, sr);
}

int whost_view_compare_ip_addr_s(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_compare_sockaddr_s(l->host, r->host);
}

int whost_view_compare_mac_addr_s(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_compare_mac_addr_s(l->host, r->host);
}

int whost_view_compare_last_seen_online_s(const whost_view_t* l,
	const whost_view_t* r)
{
	time_t tL = whost_get_last_seen_online_s(l->host);
	time_t tR = whost_get_last_seen_online_s(r->host);
	return (int)(tL - tR);
}

int whost_view_compare_op_result_s(const whost_view_t* l,
	const whost_view_t* r)
{
	whost_op_result_t orL = whost_get_op_result_s(l->host);
	whost_op_result_t orR = whost_get_op_result_s(r->host);
	return orL - orR;
}


BOOL whost_view_update_host_s(whost_view_t* hw,
	const whost_t* that_host)
{
	const char* thatHostname = whost_get_hostname_s(that_host);
	whost_view_set_hostname_s(hw, thatHostname);
	whost_view_set_sockarrd_s(hw, whost_get_sockaddr_s(that_host));
	whost_view_set_last_seen_online_s(hw, whost_get_last_seen_online_s(that_host));

	return whost_view_get_state_s(hw) != cf_unchanged;
}


BOOL whost_view_set_display_name_s(whost_view_t* hw,
	const char* displayName)
{
	return whost_set_display_name(hw->host, displayName);
}

BOOL whost_view_set_hostname_s(whost_view_t* hw,
	const char* hostName)
{
	return whost_set_hostname_s(hw->host, hostName);
}

BOOL whost_view_set_ip_addr_s(whost_view_t* hw,
	const char* ipAddrString)
{
	BOOL isSet = whost_set_ip_addr_s(hw->host, ipAddrString);

	if (isSet) {
		wfree_s(hw->ipAddrString);
		hw->ipAddrString = NULL;
	}

	return isSet;
}

BOOL whost_view_set_sockarrd_s(whost_view_t* hw,
	const struct sockaddr* sa)
{
	BOOL isSet = whost_set_sockaddr_s(hw->host, sa);

	if (isSet) {
		wfree_s(hw->ipAddrString);
		hw->ipAddrString = NULL;
	}

	return isSet;
}

BOOL whost_view_set_sockaddr_int_s(whost_view_t* hw,
	uint32_t addr)
{
	BOOL isSet = whost_set_sockaddr_int_s(hw->host, addr);

	if (isSet) {
		wfree_s(hw->ipAddrString);
		hw->ipAddrString = NULL;
	}

	return isSet;
}

BOOL whost_view_set_mac_addr_s(whost_view_t* hw,
	const byte* mac_addr)
{
	BOOL isSet = whost_set_mac_addr_s(hw->host, mac_addr);

	if (isSet) {
		wfree_s(hw->macAddrString);
		hw->macAddrString = NULL;
	}

	return isSet;
}

BOOL whost_view_set_mac_addr_string_s(whost_view_t* hw, const char* const restrict mac_addr_str)
{
	byte* mac_addr = wmac_addr_from_string_s(mac_addr_str);
	BOOL isSet = whost_view_set_mac_addr_s(hw, mac_addr);
	wmac_addr_free_s(mac_addr);

	return isSet;
}

BOOL whost_view_set_last_seen_online_s(whost_view_t* hw,
	time_t lastSeenOnlineString)
{
	BOOL isSet = whost_set_last_seen_online_s(hw->host, lastSeenOnlineString);

	if (isSet) {
		wfree_s(hw->lastSeenOnlineString);
		hw->lastSeenOnlineString = NULL;
	}

	return isSet;
}

BOOL whost_view_set_op_result_s(whost_view_t* hw,
	whost_op_result_t or)
{
	return whost_set_op_result(hw->host, or);
}

void whost_view_set_extra_data_s(whost_view_t* hw,
	const void* data, size_t size)
{
	wfree_s(hw->extraData);

	if (data && size) {
		hw->extraData = walloc_s(size);
		if (hw->extraData) {
			memcpy(hw->extraData, data, size);
		}
		else {
			wlog_if_level(wll_error, "No %zuB of free memory for extra data; extra data haven't stored :-(\n",
				size);
		}
	}
	else {
		hw->extraData = NULL;
	}
}

static void update_whost_view_ip_addr(whost_view_t* const restrict hw)
{
	const struct sockaddr* sa = whost_get_sockaddr_s(hw->host);

	if (hw->ipAddrString) {
		wfree_s(hw->ipAddrString);
		hw->ipAddrString = NULL;
	}

	if (!sa) {
		wlog_if_level(wll_verbose, "Null socket address passed to %s\n",
			__func__);
		return;
	}

	if (sa->sa_family != AF_INET && sa->sa_family != AF_INET6) {
		wlog_if_level(wll_verbose, "The %d address family is neither AF_INET nor AF_INET6\n",
			sa->sa_family);
		return;
	}

	socklen_t buf_size = sa->sa_family == AF_INET6 ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;

	hw->ipAddrString = walloc_s(buf_size);

	if (!hw->ipAddrString) {
		wlog_if_level(wll_warning, "No %zuB of free memory for IP address\n", buf_size);
		return;
	}

	wsockaddr_to_string_s(sa, hw->ipAddrString, buf_size);
}

static void update_whost_view_mac_addr(whost_view_t* const restrict hw)
{
	static const size_t buf_size = 3 * ETHER_ADDR_LEN;

	if (hw->macAddrString) {
		wfree_s(hw->macAddrString);
		hw->macAddrString = NULL;
	}

	const byte* mac_addr = whost_get_mac_addr_s(hw->host);
	if (wmac_addr_is_empty_s(mac_addr)) {
		return;
	}

	hw->macAddrString = walloc_s(buf_size);

	if (!hw->macAddrString) {
		wlog_if_level(wll_warning, "No %zuB of free memory for MAC addr\n", buf_size);
		return;
	}

	wmac_addr_to_string_s(mac_addr, hw->macAddrString);
}

static void update_whost_view_last_seen_online(whost_view_t* hw,
	time_t lastSeenOnline)
{
	static const char* format = "%Y.%m.%d %H:%M:%S";
	static const size_t buf_size = 24;

	if (hw->lastSeenOnlineString) {
		wfree_s(hw->lastSeenOnlineString);
		hw->lastSeenOnlineString = NULL;
	}

	const struct tm* tm = localtime(&lastSeenOnline);


	if (tm) {
		hw->lastSeenOnlineString = walloc_s(buf_size);
		if (!hw->lastSeenOnlineString) {
			wlog_if_level(wll_warning, "No %zuB of free memory for lst seen online buffer\n",
				buf_size);
			return;
		}

		strftime(hw->lastSeenOnlineString, buf_size, format, tm);
	}
}

void whost_view_set_field_change_processed_s(const whost_view_t* hw,
	whost_changed_field_t cf)
{
	whost_set_field_change_processed_s(hw->host, cf);
}

whost_changed_field_t whost_field_2_changed_field_s(whost_field_t hf)
{
	whost_changed_field_t cf = cf_unchanged;

	switch (hf)
	{
	case hwf_display_name:
		cf = cf_display_name;
		break;
	case hwf_host_name:
		cf = cf_host_name;
		break;
	case hwf_ip_address:
		cf = cf_ip_address;
		break;
	case hwf_mac_address:
		cf = cf_mac_address;
		break;
	case hwf_last_seen_online:
		cf = cf_last_seen_online;
		break;
	case hwf_op_result:
		cf = cf_op_result;
		break;
	default:
		wlog_if_level(wll_error, "Invalid value '%d' passed to %s\n",
			hf, __func__);
	}

	return cf;
}
