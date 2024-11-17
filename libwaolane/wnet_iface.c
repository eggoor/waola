/** @file wnet_iface.c
* Waola network interface wrapper implementation.
*/

#include "pch.h"

#include "waolane_fact4s.h"
#include "warper.h"

#include "wnet_iface.h"

struct wnet_iface {
	const waolane_fact4s_t* fact4s;
	char* displayName;
	struct sockaddr* sockAddr;
	int index;
	uint32_t mask;
	warper_t* warper;
	byte macAddr[ETHER_ADDR_LEN];
};

wnet_iface_t* wnet_iface_make_impl(const waolane_fact4s_t* fact4s,
	int ifindex, const char* if_name, struct sockaddr* sa,
	uint32_t mask, const byte* mac_addr,
	wthread_counter_t* tc)
{
	wnet_iface_t* nif = walloc_s(sizeof(struct wnet_iface));
	if (!nif)
	{
		wlog_if_level(wll_warning, "No %zuB of free memory for network interface\n",
			sizeof(struct wnet_iface));
		goto end;
	}

	nif->displayName = wstrdup_s(if_name);
	if (!nif->displayName && if_name) {
		wlog_if_level(wll_warning, "No %zuB of free memory for network interface display name\n",
			strlen(if_name));
	}

	nif->sockAddr = wsockaddr_dup_s(sa);
	if (!nif->sockAddr) {
		wfree_s(nif->displayName);
		wfree_s(nif);
		nif = NULL;
		wlog_if_level(wll_warning, "Unable to duplicate socket address\n");
		goto end;
	}

	nif->warper = warper_make_impl(fact4s, tc);
	
	if (!nif->warper) {
		wfree_s(nif->sockAddr);
		wfree_s(nif->displayName);
		wfree_s(nif);
		nif = NULL;
		wlog_if_level(wll_warning, "%s: can't create ARP client\n",
			__func__);
		goto end;
	}

	nif->fact4s = fact4s;
	nif->index = ifindex;
	nif->mask = mask;

	if (mac_addr) {
		memcpy(nif->macAddr, mac_addr, ETHER_ADDR_LEN);
	}
	else {
		memset(nif->macAddr, 0, ETHER_ADDR_LEN);
	}

end:
	return nif;
}

void wnet_iface_free(wnet_iface_t* self)
{
	warper_free(self->warper);
	wfree_s(self->displayName);
	wfree_s(self->sockAddr);
	wfree_s(self);
}

const waolane_fact4s_t* wnet_iface_get_factories(const wnet_iface_t* self)
{
	return self->fact4s;
}

int wnet_iface_get_index(const wnet_iface_t* self)
{
	return self->index;
}

sa_family_t wnet_iface_get_addr_family(const wnet_iface_t* self)
{
	return self->sockAddr->sa_family;
}

struct sockaddr* wnet_iface_get_sockaddr(const wnet_iface_t* self)
{
	return self->sockAddr;
}

warper_t* wnet_iface_get_arper(const wnet_iface_t* self)
{
	return self->warper;
}

uint32_t wnet_iface_get_self_ip_addr(const wnet_iface_t* self)
{
	uint32_t ip_addr = 0;

	if (AF_INET == self->sockAddr->sa_family) {
		ip_addr = ((struct sockaddr_in*)self->sockAddr)->sin_addr.s_addr;
	}

	return ip_addr;
}

size_t wnet_iface_get_sockaddr_len(const wnet_iface_t* self)
{
	size_t len = 0;

	switch (self->sockAddr->sa_family)
	{
	case AF_INET:
		len = sizeof(struct sockaddr_in);
		break;
	case AF_INET6:
		len = sizeof(struct sockaddr_in6);
		break;
	default:
		wlog_if_level(wll_warning, "Unsupperted socket family %d in %s\n",
			self->sockAddr->sa_family, __func__);
	}

	return len;
}

uint32_t wnet_iface_get_subnet_mask(const wnet_iface_t* self)
{
	return self->mask;
}

const char* wnet_iface_get_name(const wnet_iface_t* self)
{
	return self->displayName;
}

const byte* wnet_iface_get_mac_addr(const wnet_iface_t* self)
{
	return self->macAddr;
}

int wnet_iface_compare(const wnet_iface_t* self, int ifindex,
	const char* ifname, sa_family_t sa_family)
{
	int rc = self->index - ifindex;
	if (rc) {
		goto end;
	}

	rc = strcmp(self->displayName, ifname);
	if (rc) {
		goto end;
	}

	rc = self->sockAddr->sa_family - sa_family;

end:
	return rc;
}

int wnet_iface_start_listening(wnet_iface_t* self, warper_cb_data_t* const restrict arp_cbd)
{
	return warper_start_listening(self->warper, arp_cbd);
}

BOOL wnet_iface_is_ready_for_arp(const wnet_iface_t* self)
{
	return warper_is_ready_for_arp(self->warper);
}

int wnet_iface_get_errno(const wnet_iface_t* self)
{
	return warper_get_errno(self->warper);
}

int wnet_iface_resolve_ip_2_mac(const wnet_iface_t* self,
	uint32_t target_ip_addr, byte* mac_addr)
{
	int rc = 0;
	uint32_t self_ip_addr = wnet_iface_get_self_ip_addr(self);

	if (self_ip_addr) {
		rc = warper_resolve_ip_2_mac(self_ip_addr, target_ip_addr, mac_addr);
	}
	else {
		rc = EPROTO;
	}

	return rc;
}

void wnet_iface_wait_for_warper_listening_finished(const wnet_iface_t* self,
	BOOL join_thread)
{
	warper_wait_for_listening_finished(self->warper, join_thread);
}

BOOL wnet_iface_is_ip_in_subnet(const wnet_iface_t* self, uint32_t ip_addr)
{
	BOOL res = FALSE;

	if (AF_INET == self->sockAddr->sa_family) {
		const struct sockaddr_in* sin = (const struct sockaddr_in*)self->sockAddr;
		if ((sin->sin_addr.s_addr & self->mask) == (ip_addr & self->mask)) {
			res = TRUE;
		}
	}
	
	return res;
}

int wnet_iface_query_arp(const wnet_iface_t* self, uint32_t ip_addr)
{
	return warper_query_arp(self->warper, ip_addr);
}

void wnet_iface_set_requesting_finished(const wnet_iface_t* self) {
	warper_set_requesting_finished(self->warper);
}
