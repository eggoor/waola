#include "pch.h"

#include "include/libwaolash.h"

#include "wsockaddr.h"
#include "whost_data.h"

#include "whost_priv.h"

whost_t* whost_make_impl(const char* displayName, const char* hostName,
	const struct sockaddr* sa, const byte* const macAddr, time_t lso)
{
	whost_t* host = walloc_s(sizeof(struct whost));

	if (!host) {
		wlog_if_level(wll_warning, "No %zuB of free memory for a host\n",
			sizeof(struct whost));
		goto end;
	}

	host->fieldState = cf_unchanged;

	host->displayName = wstrdup_s(displayName);

	if (host->displayName) {
		host->fieldState |= cf_display_name;
	}

	host->hostname = wstrdup_s(hostName);

	if (host->hostname) {
		host->fieldState |= cf_host_name;
		if (!host->displayName) {
			host->fieldState |= cf_display_name;
		}
	}

	if (sa) {
		whost_do_set_sockaddr(host, sa);
		if (host->sockAddr) {
			host->fieldState |= cf_ip_address;
		}
	}
	else {
		host->sockAddr = NULL;
	}

	if (macAddr) {
		memcpy(host->macAddr, macAddr, ETHER_ADDR_LEN);
		host->fieldState |= cf_mac_address;
	}
	else {
		memset(host->macAddr, 0, ETHER_ADDR_LEN);
	}

	host->lastSeenOnline = lso;
	if (host->lastSeenOnline != 0) {
		host->fieldState |= cf_last_seen_online;
	}

	host->opResult = wor_undefined;

end:
	return host;
}

whost_t* whost_from_data_impl(const whost_data_t* const hd)
{
	whost_t* host = NULL;

	struct sockaddr* sa = wsockaddr_from_string_s(hd->ipAddr);
	byte* mac_addr = wmac_addr_from_string_s(hd->macAddr);

	host = whost_make_impl(hd->displayName, hd->hostName,
		sa, mac_addr, hd->lastSeenOnline);

	wmac_addr_free_s(mac_addr);
	wfree_s(sa);

	return host;
}

BOOL whost_data_is_empty_s(const whost_data_t* self)
{
	return !self || (wstr_is_empty_s(self->macAddr) && wstr_is_empty_s(self->ipAddr) 
		&& wstr_is_empty_s(self->hostName) && wstr_is_empty_s(self->displayName));
}

void whost_free(whost_t* const restrict host)
{
	wfree_s(host->displayName);
	wfree_s(host->hostname);
	wfree_s(host->sockAddr);
	wfree_s(host);
}

const char* whost_get_display_name_s(const whost_t* const restrict host)
{
	return host->displayName;
}

const char* whost_get_hostname_s(const whost_t* const restrict host)
{
	return host->hostname;
}

const struct sockaddr* whost_get_sockaddr_s(const whost_t* const restrict host)
{
	return host->sockAddr;
}

const byte* whost_get_mac_addr_s(const whost_t* const restrict host)
{
	return host->macAddr;
}

time_t whost_get_last_seen_online_s(const whost_t* const restrict host)
{
	return host->lastSeenOnline;
}

whost_changed_field_t whost_get_state_s(const whost_t* const restrict host)
{
	return host->fieldState;
}

BOOL whost_is_updated_s(const whost_t* const restrict host)
{
	return whost_get_state_s(host) != cf_unchanged;
}

whost_op_result_t whost_get_op_result_s(const whost_t* const restrict host)
{
	return host->opResult;
}

BOOL whost_is_field_updated_s(const whost_t* const restrict host, whost_field_t hf)
{
	BOOL updated = FALSE;

	switch (hf)
	{
	case hwf_display_name:
		updated = host->fieldState & cf_display_name;
		break;
	case hwf_host_name:
		updated = host->fieldState & cf_host_name;
		break;
	case hwf_ip_address:
		updated = host->fieldState & cf_ip_address;
		break;
	case hwf_mac_address:
		updated = host->fieldState & cf_mac_address;
		break;
	case hwf_last_seen_online:
		updated = host->fieldState & cf_last_seen_online;
		break;
	case hwf_op_result:
		updated = host->fieldState & cf_op_result;
		break;
	default:
		wlog_if_level(wll_warning, "Invalid host field %d passed to %s\n",
			hf, __func__);
	}

	return updated;
}

BOOL whost_set_sockaddr_s(whost_t* host, const struct sockaddr* const restrict sa)
{
	if (wsockaddr_compare_s(host->sockAddr, sa) == 0) {
		return FALSE;
	}
	else {
		host->fieldState |= cf_ip_address;
	}

	if (host->sockAddr) {
		wfree_s(host->sockAddr);
	}

	if (sa) {
		whost_do_set_sockaddr(host, sa);
	}
	else {
		host->sockAddr = NULL;
	}

	return TRUE;
}

BOOL whost_set_sockaddr_int_s(whost_t* const restrict host, uint32_t addr)
{
	struct sockaddr_in sa = {
		.sin_family = AF_INET,
		.sin_port = 0,
		.sin_addr.s_addr = addr
	};

	return whost_set_sockaddr_s(host, (const struct sockaddr*)&sa);
}

BOOL whost_set_ip_addr_s(whost_t* host, const char* const restrict ipAddrString)
{
	BOOL isSet = FALSE;
	struct sockaddr* sa = wsockaddr_from_string_s(ipAddrString);

	if (sa) {
		isSet = whost_set_sockaddr_s(host, sa);
		wfree_s(sa);
	}

	return isSet;
}

BOOL whost_set_mac_addr_s(whost_t* host, const byte* const restrict mac)
{
	if (wmac_addr_compare_s(host->macAddr, mac) == 0) {
		return FALSE;
	}
	else {
		host->fieldState |= cf_mac_address;
	}

	if (mac) {
		memcpy(host->macAddr, mac, ETHER_ADDR_LEN);
	}
	else {
		memset(host->macAddr, 0, ETHER_ADDR_LEN);
	}

	return TRUE;
}

BOOL whost_set_last_seen_online_s(whost_t* const restrict host, time_t lso)
{
	if (host->lastSeenOnline == lso) {
		return FALSE;
	}
	
	host->lastSeenOnline = lso;
	host->fieldState |= cf_last_seen_online;
	
	return TRUE;
}

BOOL whost_set_hostname_s(whost_t* host, const char* const restrict hostName)
{
	if (is_better_hostname(host, hostName)) {
		host->fieldState |= cf_host_name;
		if (!host->displayName) {
			host->fieldState |= cf_display_name;
		}
	}
	else {
		return FALSE;
	}

	if (host->hostname) {
		wfree_s(host->hostname);
	}

	host->hostname = wstrdup_s(hostName);

	return TRUE;
}

BOOL whost_set_display_name(whost_t* host, const char* const restrict displayName)
{
	if (!whost_needs_set_display_name(host, displayName)) {
		return FALSE;
	}
	else {
		host->fieldState |= cf_display_name;
	}

	if (host->displayName) {
		wfree_s(host->displayName);
	}

	host->displayName = wstrdup_s(displayName);

	return TRUE;
}

BOOL whost_set_op_result(whost_t* const restrict host, const whost_op_result_t opr)
{
	if (opr == host->opResult) {
		return FALSE;
	}
	else {
		host->fieldState |= cf_op_result;
	}

	host->opResult = opr;

	return TRUE;
}


int whost_compare_s(const whost_t* stored, const whost_t* const restrict discovered)
{
	int res = 0;

	if (stored != discovered) {
		if (!wmac_addr_is_empty_s(stored->macAddr)
			&& !wmac_addr_is_empty_s(discovered->macAddr)) {
			res = whost_compare_mac_addr_s(stored, discovered);
		}
		else if (!wsockaddr_addr_is_empty_s(stored->sockAddr)
			&& !wsockaddr_addr_is_empty_s(discovered->sockAddr))
		{
			res = whost_compare_sockaddr_s(stored, discovered);
		}
		else if (!wstr_is_empty_s(stored->hostname)
			&& !wstr_is_empty_s(discovered->hostname)) {
			res = whost_compare_hostname_s(stored, discovered);
		}
		else if (!wstr_is_empty_s(stored->displayName)
			&& !wstr_is_empty_s(discovered->displayName)) {
			res = whost_compare_display_name_s(stored, discovered);
		}
		else {
			res = wmac_addr_compare_s(stored->macAddr, discovered->macAddr);

			if (0 == res) {
				res = wsockaddr_compare_s(stored->sockAddr, discovered->sockAddr);
			}

			if (0 == res) {
				if (!wstr_is_empty_s(stored->hostname)
					&& !wstr_is_empty_s(discovered->hostname)) {
					res = whost_compare_hostname_s(stored, discovered);
				}
				else {
					res = (int)(stored->hostname - discovered->hostname);
				}
			}
			if (0 == res) {
				if (!wstr_is_empty_s(stored->displayName)
					&& !wstr_is_empty_s(discovered->displayName)) {
					res = whost_compare_display_name_s(stored, discovered);
				}
				else {
					res = (int)(stored->displayName - discovered->displayName);
				}
			}
		}
	}

	return res;
}

int whost_compare_display_name_s(const whost_t* l, const whost_t* const restrict r)
{
	return wstrcmp_s(l->displayName, r->displayName);
}

int whost_compare_hostname_s(const whost_t* l, const whost_t* const restrict r)
{
	return wstrcmp_s(l->hostname, r->hostname);
}

int whost_compare_sockaddr_s(const whost_t* l, const whost_t* const restrict r)
{
	return wsockaddr_compare_s(l->sockAddr, r->sockAddr);
}

int whost_compare_mac_addr_s(const whost_t* l, const whost_t* const restrict r)
{
	return wmac_addr_compare_s(l->macAddr, r->macAddr);
}

void whost_set_field_change_processed_s(whost_t* const restrict host, whost_changed_field_t cf)
{
	if (cf & host->fieldState) {
		host->fieldState &= ~cf;
	}
}

static BOOL whost_needs_set_display_name(whost_t* host, const char* const restrict displayName)
{
	return wstrcmp_s(host->displayName, displayName) != 0
		&& !(!host->displayName && wstrcmp_s(host->hostname, displayName) == 0);
}

static void whost_do_set_sockaddr(whost_t* host, const struct sockaddr* const restrict sa)
{
	host->sockAddr = wsockaddr_dup_s(sa);
}

static BOOL is_better_hostname(const whost_t* host, const char* const restrict hostName)
{
	if (wstr_is_empty_s(hostName)) {
		return FALSE;
	}

	char ip_addr_string[INET6_ADDRSTRLEN];
	if (wstrcmp_s(hostName, wsockaddr_to_string_s(host->sockAddr,
		ip_addr_string, INET6_ADDRSTRLEN)) == 0) {
		return FALSE;
	}

	return wstrcmp_s(host->hostname, hostName) != 0;
}
