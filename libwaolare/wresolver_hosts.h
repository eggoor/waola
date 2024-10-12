#pragma once

#include "include/wresolver_hosts.h"

#include "waolare_fact4s.h"

struct wresolver_hosts
{
	const waolare_t* parent;
	wthread_counter_t* threadCounter;
	wthread_cbi_t* threadCbi;
	wcont4r_t* resolvers;
	waolast_t* status;
	BOOL isAsync;
	BOOL cancelled;
	BOOL finishing;
};

const waolare_fact4s_t* wresolver_hosts_get_fact4s(const wresolver_hosts_t* self);
int wresolver_hosts_on_resolving_finished(wresolver_hosts_t* self);
void wresolver_hosts_query_arp(wresolver_hosts_t* self, const whost_view_t* const restrict hw);
void wresolver_hosts_on_arp_response(warper_cb_data_t* const restrict data);
wnet_iface_t* wresolver_hosts_find_network_iface(const wresolver_hosts_t* self,
	uint32_t ip_addr);
