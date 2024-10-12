#pragma once

#include "include/wnet_iface_prov.h"

struct wnet_iface_prov
{
	waolane_fact4s_t* fact4s;
	wcont4r_t* ifList;
	wcont4r_t* arp_cache;
	wthread_counter_t* threadCounter;
	BOOL collectionCancelled;
};

BOOL is_collection_net_ifaces_cancelled(const wnet_iface_prov_t* self);
BOOL wnet_iface_prov_is_already_collected(const wnet_iface_prov_t* self,
	int ifindex, const char* const restrict ifname, sa_family_t sa_family);
void wnet_iface_prov_collect_ifaces(wnet_iface_prov_t* self);
BOOL wnet_iface_prov_process_cache(wcont4r_t* const restrict arp_entries);
