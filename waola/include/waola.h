/** @file libwaola.h
* Wake on LAN library for dynamic linking.
*/

#pragma once

#include "waola_exports.h"

#if __cplusplus
extern "C" {
#endif

/**
 * Sets log level.
 *
 * @param level log level.
 */
WAOLAD_API void waola_set_log_level(wlog_level_t level);

/**
 * Wakes up host(s) with provided MAC address(es).
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
WAOLAD_API int waolam(const wcont4r_factory cont4r_factory, const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory, const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory, const wnet_iface_prov_factory net_iface_prov_factory,
	const wthread_counter_factory thread_counter_factory, const char* const mac_addresses[], size_t count);

/**
 * Wakes up given hosts.
 * Broadcasts magic packet for each specified host's
 * MAC address through each active network interface.
 *
 * @param hosts array of target hosts.
 * @param count host count.
 *
 * @return 0 on success, otherwise error code.
 */
WAOLAD_API int waolah(whost_view_t* const hosts[], size_t count);

#if __cplusplus
}   // Extern C
#endif
