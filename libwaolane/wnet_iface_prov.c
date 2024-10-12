#include "pch.h"

#include "include/arp_cache_entry.h"

#include "waolane_fact4s.h"
#include "wnet_iface.h"

#include "wnet_iface_prov.h"

static void cancel_collect_wnet_ifaces(wnet_iface_prov_t* self);

const wcont4r_t* wnet_iface_prov_get_collected_ifaces(wnet_iface_prov_t* self)
{
	if (!self->ifList) {
		wnet_iface_prov_collect_ifaces(self);
	}

	return self->ifList;
}

wnet_iface_prov_t* wnet_iface_prov_make_impl(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory, const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory, const wnet_iface_factory net_iface_factory,
	wthread_counter_t* tc)
{
	wnet_iface_prov_t* nip = walloc_s(sizeof(struct wnet_iface_prov));

	if (!nip) {
		wlog_if_level(wll_warning, "No %zuB of free memory for network interface provider\n",
			sizeof(struct wnet_iface_prov));
		goto end;
	}

	nip->fact4s = wmake_waolane_fact4s(cont4r_factory, mutex_factory,
		thread_cbi_factory, thread_factory, net_iface_factory);

	if (!nip->fact4s) {
		wfree_s(nip);
		wlog_if_level(wll_warning, "Not enough free memory for network interface provider factories\n");
		goto end;
	}
	
	nip->ifList = NULL;
	nip->arp_cache = NULL;
	nip->threadCounter = tc;

end:
	return nip;
}

void wnet_iface_prov_free(wnet_iface_prov_t* self)
{
	cancel_collect_wnet_ifaces(self);
	
	wnet_iface_t* nif = wcont4r_get_first_s(self->ifList);
	while (nif) {
		wnet_iface_free(nif);
		nif = wcont4r_get_next_s(self->ifList);
	}
	wcont4r_free_s(self->ifList);
	
	if (self->arp_cache) {
		arp_cache_entry_t* ce = wcont4r_get_first_s(self->arp_cache);
		while (ce) {
			wfree_s(ce);
			ce = wcont4r_get_next_s(self->arp_cache);
		}
		wcont4r_free_s(self->arp_cache);
	}
	
	waolane_fact4s_free(self->fact4s);
	wfree_s(self);
}

void (*wnet_iface_prov_get_canceller(wnet_iface_prov_t* self))(wnet_iface_prov_t* self)
{
	return cancel_collect_wnet_ifaces;
}

BOOL is_collection_net_ifaces_cancelled(const wnet_iface_prov_t* self)
{
	return self->collectionCancelled;
}

static void cancel_collect_wnet_ifaces(wnet_iface_prov_t* self)
{
	self->collectionCancelled = TRUE;
}

BOOL wnet_iface_prov_is_already_collected(const wnet_iface_prov_t* self,
	int ifindex, const char* const restrict ifname, sa_family_t sa_family)
{
	const wnet_iface_t* nif = wcont4r_get_first_s(self->ifList);

	while (nif) {
		if (wnet_iface_compare(nif, ifindex, ifname, sa_family) == 0) {
			return TRUE;
		}
		nif = wcont4r_get_next_s(self->ifList);
	}

	return FALSE;
}

wnet_iface_t* wnet_iface_prov_make_dummy(const wnet_iface_prov_t* self)
{
	if (!self->fact4s) {
		wlog_if_level(wll_warning, "Network interface provider has no factories\n");
		return NULL;
	}

	return (* self->fact4s->make_net_iface)(self->fact4s,
		0, "dummy",	NULL, 0, NULL, self->threadCounter);
}

uint32_t wnet_iface_prov_lookup_ip_addr_in_arp_cache(const wnet_iface_prov_t* self,
	const byte* mac_addr)
{
	uint32_t ip_addr = 0;

	if (self->arp_cache) {
		arp_cache_entry_t* ce = wcont4r_get_first_s(self->arp_cache);
		while (ce) {
			if (wmac_addr_compare_s(mac_addr, ce->macAddr) == 0) {
				ip_addr = ce->ipAddr;
				break;
			}
			ce = wcont4r_get_next_s(self->arp_cache);
		}
	}

	return ip_addr;
}

BOOL wnet_iface_prov_prepare_for_arp(wnet_iface_prov_t* self)
{
	BOOL res = FALSE;

	if (!self->ifList) {
		wnet_iface_prov_collect_ifaces(self);
	}

	wnet_iface_t* nif = wcont4r_get_first_s(self->ifList);
	while (nif) {
		int rc = wnet_iface_prepare_for_arp(nif);
		if (0 == rc) {
			if (!res) {
				res = TRUE;
			}
		}
		else {
			wlog_if_level(wll_warning, "%s: preparation for ARP failed\n",
				wnet_iface_get_name(nif));
		};
		nif = wcont4r_get_next_s(self->ifList);
	}

	self->arp_cache = (* self->fact4s->make_cont4r)(wct_list);

	if (self->arp_cache) {
		if (!wnet_iface_prov_process_cache(self->arp_cache)) {
			wlog_if_level(wll_debug, "Unable to get any entry from ARP cache\n");
			wcont4r_free_s(self->arp_cache);
			self->arp_cache = NULL;
		}
	}
	else {
		wlog_if_level(wll_warning, "Unable to make ARP cache list\n");
	}

	return res;
}

wnet_iface_t* wnet_iface_prov_find_iface(wnet_iface_prov_t* self, uint32_t ip_addr)
{
	wnet_iface_t* nif = wcont4r_get_first_s(self->ifList);
	while (nif) {
		if (wnet_iface_is_ip_in_subnet(nif, ip_addr)) {
			return nif;
		}
		nif = wcont4r_get_next_s(self->ifList);
	}
	
	return NULL;
}

const wcont4r_t* wnet_iface_prov_get_cached(const wnet_iface_prov_t* self)
{
	return self->arp_cache;
}

void wnet_iface_prov_wait_for_listening_finished(const wnet_iface_prov_t* self)
{
	wnet_iface_t* nif = wcont4r_get_first_s(self->ifList);
	while (nif) {
		wnet_iface_wait_for_listening_finished(nif, TRUE);
		nif = wcont4r_get_next_s(self->ifList);
	}
}
