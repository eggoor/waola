/** @file waolasc_factory.h
* Waola scanner factory.
*
* Creates and releases waola scanner.
*/

#pragma once

#include "waolath_fact4s_sh.h"
#include "wnet_iface_factory.h"
#include "wnet_iface_prov_factory.h"
#include "waoladi_fact4s_sh.h"
#include "waolare_fact4s_sh.h"
#include "waolava_sh.h"

#if __cplusplus
extern "C" {
#endif

typedef waolasc_t* (*wfact_waolasc)(void);

/**
 * Makes scanner using default factories.
 *
 * @param state_cbi Optional. If not NULL, the caller will be
 * subscribed for scanner state changes.
 * @param vault_cbi Optional. If not NULL, the caller will be
 * subscribed for vault operations.
 *
 * @return scanner interface pointer.
 */
waolasc_t* wmake_waolasc_def_s(const waolasc_subscribe_data_t* state_cbi,
	const waolava_subscribe_data_t* vault_cbi);

/**
 * Makes scanner using caller-provided factories.
 *
 * @param cont4r_factory container factory.
 * @param mutex_factory mutex factory.
 * @param event_factory event factory.
 * @param thread_counter_factory thread counter factory.
 * @param thread_cbi_factory thread callback info factory.
 * @param thread_factory thread factory.
 * @param net_iface_factory network interface factory.
 * @param net_iface_prov_factory network interface provider factory.
 * @param host_factory host factory.
 * @param host_from_data_factory host from data factory.
 * @param host_view_factory host view factory.
 * @param waolava_factory Waola vault factory.
 * @param waolava_on_host_modified_cbi_factory host modified callback info factory.
 * @param waolava_merge_host_cbi_factory merge host callback info factory.
 * @param waoladi_factory Waola discoverer factory.
 * @param discoverer_net_factory network discoverer factory.
 * @param discoverer_iface_factory interface discoverer factory.
 * @param discoverer_iface_monitor_factory interface discoverer monitor factory.
 * @param discoverer_host_factory host discoverer factory.
 * @param waolare_factory Waola resolver factory.
 * @param resolver_hosts_factory resolver hosts factory.
 * @param resolver_host_factory resolver host factory.
 * @param status_factory status factory.
 * @param state_cbi If not NULL, the caller will be
 * subscribed for scanner state changes.  Optional.
 * @param vault_cbi If not NULL, the caller will be
 * subscribed for vault operations. Optional.
 *
 * @return scanner interface pointer. Must bee freed by caller with @waolasc_free
 */
waolasc_t* waolasc_make_s(const wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const wevent_factory event_factory,
	const wthread_counter_factory thread_counter_factory,
	const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory,
	const wnet_iface_prov_factory net_iface_prov_factory,
	const whost_factory host_factory,
	const whost_from_data_factory host_from_data_factory,
	const whost_view_factory host_view_factory,
	const waolava_factory waolava_factory,
	const waolava_on_host_modified_cbi_factory waolava_on_host_modified_cbi_factory,
	const waolava_merge_host_cbi_factory waolava_merge_host_cbi_factory,
	const waoladi_factory waoladi_factory,
	const wdiscoverer_net_factory discoverer_net_factory,
	const wdiscoverer_iface_factory discoverer_iface_factory,
	const wdiscoverer_iface_monitor_factory discoverer_iface_monitor_factory,
	const wdiscoverer_host_factory discoverer_host_factory,
	const waolare_factory waolare_factory,
	const wresolver_hosts_factory resolver_hosts_factory,
	const wresolver_host_factory resolver_host_factory,
	const waolast_factory status_factory,
	const waolasc_subscribe_data_t* state_cbi,
	const waolava_subscribe_data_t* vault_cbi);

#if __cplusplus
}   // Extern C
#endif
