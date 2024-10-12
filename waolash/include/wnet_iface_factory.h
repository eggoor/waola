/** @file wnet_iface_factory.h
* Waola network interface factory.
*/

#pragma once

#include "wthread_counter_t.h"
#include "waolane_fact4s_t.h"
#include "wnet_iface_t.h"

#if __cplusplus
extern "C" {
#endif

/**
 * Pointer to network interface factory function.
 *
 * @param ifindex Interface index.
 * @param iface_name Interface name.
 * @param sockaddr Interface IP address.
 * @param mask Interface subnet mask.
 * @param mac_addr Interface MAC address.
 *
 * @return network interface pointer.
 */
typedef wnet_iface_t* (*wnet_iface_factory)(const waolane_fact4s_t* fact4s,
	int ifindex, const char* iface_name, struct sockaddr* sockaddr,
	uint32_t mask, const byte* mac_addr, wthread_counter_t* tc);

wnet_iface_t* wmake_net_iface(const waolane_fact4s_t* fact4s, int ifindex,
	const char* ifaceName, struct sockaddr* sockaddr, uint32_t mask,
	const byte* mac_addr, wthread_counter_t* tc);

#if __cplusplus
}   // Extern C
#endif
