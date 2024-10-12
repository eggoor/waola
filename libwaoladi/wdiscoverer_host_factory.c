#include "pch.h"

#include "waoladi_fact4s.h"

wdiscoverer_host_t* wdiscoverer_host_make_impl(const waoladi_fact4s_t* fact4s,
	wnet_iface_t* net_iface, uint32_t target_ip_addr, const byte* mac_addr,
	waolast_t* status, wthread_cbi_t* master_cbi);

wdiscoverer_host_t* wmake_discoverer_host(const waoladi_fact4s_t* fact4s,
	wnet_iface_t* net_iface, uint32_t target_ip_addr, const byte* mac_addr,
	waolast_t* status, wthread_cbi_t* master_cbi)
{
	return wdiscoverer_host_make_impl(fact4s, net_iface,
		target_ip_addr, mac_addr, status, master_cbi);
}
