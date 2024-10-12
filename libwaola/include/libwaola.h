/** @file libwaola.h
* Wake on LAN library.
*
* Broadcasts magic packet for each specified MAC
* address through all active network interfaces.
*/

#pragma once

/*
Include in precompiled header:

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WS2tcpip.h>
#endif
*/

#if __cplusplus
extern "C" {
#endif

/**
 * Wakes up host(s) with the given MAC address(es).
 * Broadcasts magic packet for each specified
 * MAC address through each active network interface.
 *
 * @param cont4r_factory container factory.
 * @param mutex_factory mutex factory.
 * @param thread_cbi_factory thread callback info factory.
 * @param thread_factory thread factory.
 * @param net_iface_factory network interface factory.
 * @param net_iface_prov_factory network interface provider factory.
 * @param thread_counter_factory thread counter factory.
 * @param mac_addresses array of target MAC addresses.
 * @param count mac_addresses size.
 *
 * @return 0 on success, otherwise error code.
 */
int waola_s(const wcont4r_factory cont4r_factory, const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory, const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory, const wnet_iface_prov_factory net_iface_prov_factory,
	const wthread_counter_factory thread_counter_factory,
	const char* const restrict mac_addresses[], size_t count);

/**
 * Wakes up given hosts.
 * Broadcasts magic packet for each specified host's
 * MAC address through each active network interface.
 * 
 * Sets host's operation result.
 *
 * @param cont4r_factory container factory.
 * @param mutex_factory mutex factory.
 * @param thread_cbi_factory thread callback info factory.
 * @param thread_factory thread factory.
 * @param net_iface_factory network interface factory.
 * @param net_iface_prov_factory network interface provider factory.
 * @param thread_counter_factory thread counter factory.
 * @param hosts array of target hosts.
 * @param count host count.
 *
 * @return 0 on success, otherwise error code.
 */
int waolah_s(const wcont4r_factory cont4r_factory, const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory, const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory, const wnet_iface_prov_factory net_iface_prov_factory,
	const wthread_counter_factory thread_counter_factory, whost_view_t* const restrict hosts[], size_t count);

#if __cplusplus
}   // Extern C
#endif
