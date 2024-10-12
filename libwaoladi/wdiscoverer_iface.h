/** @file wdiscoverer_iface.h
*
* Network interface scanner.
*/

#pragma once

#include "wdiscoverer_net.h"
#include "wdiscoverer_host.h"

int wdiscoverer_iface_free(wdiscoverer_iface_t* self, BOOL thread_callback);

const char* wdiscoverer_iface_get_name(const wdiscoverer_iface_t* self);
wdiscoverer_net_t* wdiscoverer_iface_get_parent(const wdiscoverer_iface_t* self);
wcont4r_t* wdiscoverer_iface_get_host_discoverer_list(const wdiscoverer_iface_t* self);
uint32_t wdiscoverer_iface_get_currently_scheduling_addr(const wdiscoverer_iface_t* self);
wnet_iface_t* wdiscoverer_iface_get_net_iface(const wdiscoverer_iface_t* self);
const waoladi_fact4s_t* wdiscoverer_iface_get_fact4s(const wdiscoverer_iface_t* self);
waolast_t* wdiscoverer_iface_get_status(const wdiscoverer_iface_t* self);
void wdiscoverer_iface_set_curr_sched_addr(wdiscoverer_iface_t* self, uint32_t ip_addr);
void wdiscoverer_iface_on_all_arp_requests_sent(wdiscoverer_iface_t* self);
uint32_t wdiscoverer_iface_get_thread_count(const wdiscoverer_iface_t* self);
void wdiscoverer_iface_notify_discovery_scheduling_finished(wdiscoverer_iface_t* self);
wdiscoverer_host_t* wdiscoverer_iface_make_host_discoverer(wdiscoverer_iface_t* self,
	uint32_t target_ip_addr, const byte* const restrict target_mac_addr);
BOOL wdiscoverer_iface_discover(wdiscoverer_iface_t* self);
int wdiscoverer_iface_do_discover(const wthread_cbi_t* const restrict cbi);
void wdiscoverer_iface_on_host_discovered(const wdiscoverer_iface_t* self, uint32_t ip_addr);
wthread_cbi_t* wdiscoverer_iface_get_thread_cbi(const wdiscoverer_iface_t* self);
