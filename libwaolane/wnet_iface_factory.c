#include "pch.h"

#include "include/wnet_iface.h"

wnet_iface_t* wnet_iface_make_impl(const waolane_fact4s_t* fact4s,
	int ifindex, const char* ifaceName, struct sockaddr* sockaddr,
	uint32_t mask, const byte* mac_addr,
	wthread_counter_t* tc);

wnet_iface_t* wmake_net_iface(const waolane_fact4s_t* fact4s,
	int ifindex, const char* ifaceName, struct sockaddr* sockaddr,
	uint32_t mask, const byte* mac_addr,
	wthread_counter_t* tc)
{
	return wnet_iface_make_impl(fact4s, ifindex,
		ifaceName, sockaddr, mask, mac_addr, tc);
}
