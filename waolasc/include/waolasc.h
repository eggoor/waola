/** @file waolasc.h
* Waola network scanner library for dynamic linking.
*
* Until otherwise specified, for both passed as in-parameters
* and for returned values:
* - ownership isn't transferrend for const pointers;
* - ownership is transferrend for non-const pointers, excepting 'self' one.
*/

#pragma once

#include <waolath_fact4s_sh.h>
#include <waolava_factory.h>
#include <waolava_callback.h>
#include <waoladi_factory.h>
#include <waolare_factory.h>
#include <waolast_factory.h>
#include <wmodule.h>
#include <waolasc_t.h>
#include <waolasc_callback.h>
#include <waolasc_opcode.h>

#include "waolasc_exports.h"

#if __cplusplus
extern "C" {
#endif

/**
 * Sets scanner's log level.
 *
 * @param level log level.
 */
WAOLACOD_API void waolasc_set_log_level(wlog_level_t level);

/**
 * Makes scanner using default factories.
 *
 * @param state_cbi If not NULL, the caller will be
 * subscribed for scanner state changes.  Optional.
 * @param vault_cbi If not NULL, the caller will be
 * subscribed for vault operations. Optional.
 *
 * @return scanner interface pointer. Must bee freed by caller with @waolasc_free
 */
WAOLACOD_API waolasc_t* wmake_waolasc_def(const waolasc_subscribe_data_t* state_cbi,
	const waolava_subscribe_data_t* vault_cbi);

/**
 * Frees scanner.
 *
 * @param self scanner interface pointer.
 */
WAOLACOD_API void waolasc_free(waolasc_t* const self);

/**
 * Subscribes for scanner's state changes.
 *
 * @param self scanner interface pointer.
 * @param waolasc_cbi subscription callback.
 *
 * @return TRUE if the caller has successfully subscribed,
 * FALSE if callback is already registered.
 */
WAOLACOD_API BOOL waolasc_subscribe_state(waolasc_t* const self,
	const waolasc_subscribe_data_t* waolasc_cbi);

/**
 * Unsubscribes from scanner's state changes.
 *
 * @param self scanner interface pointer.
 * @param waolasc_cbi callback to unsubscribe.
 *
 * @return TRUE if the caller has successfully unsubscribed,
 * FALSE if callback wasn't registered.
 */
WAOLACOD_API BOOL waolasc_unsubscribe_state(waolasc_t* const self,
	const waolasc_subscribe_data_t* waolasc_cbi);

/**
 * Subscribes for vault's operations.
 *
 * @param self scanner interface pointer.
 * @param vault_cbi vault's operations callback.
 *
 * @return TRUE if the caller has successfully subscribed,
 * FALSE if callback is already registered.
 */
WAOLACOD_API BOOL waolasc_subscribe_vault(waolasc_t* const self,
	const waolava_subscribe_data_t* vault_cbi);

/**
 * Unsubscribes from vault's operations.
 *
 * @param self scanner interface pointer.
 * @param vault_cbi callback to unsubscribe.
 *
 * @return TRUE if the caller has successfully unsubscribed,
 * FALSE if callback wasn't registered.
 */
WAOLACOD_API BOOL waolasc_unsubscribe_vault(waolasc_t* const self,
	const waolava_subscribe_data_t* vault_cbi);

/**
 * Checks if there is any discovered host.
 *
 * @param self scanner interface pointer.
 *
 * @return TRUE if there is no any discovered host.
 */
WAOLACOD_API BOOL waolasc_is_empty(const waolasc_t* const self);

/**
 * Gets currently running task flag(s).
 *
 * @param self scanner interface pointer.
 *
 * @return currently running task flag(s).
 */
WAOLACOD_API wtask_t waolasc_get_current_tasks(const waolasc_t* const self);

/**
 * Sets scanner's maximum thread count.
 *
 * @param self scanner interface pointer.
 * @param max_threads thread count limit.
 */
WAOLACOD_API void waolasc_set_max_threads(waolasc_t* const self, uint32_t max_threads);

/**
 * Discovers all possible hosts against all active network interfaces.
 * Notifies subscribed clients about each discovered host
 * with vault's callback.
 *
 * @param self scanner interface pointer.
 *
 * @return 0 on success, otherwise error code.
 */
WAOLACOD_API int waolasc_discover(waolasc_t* const self);

/**
 * Starts asynchronous discovery.
 *
 * @param self scanner interface pointer.
 *
 * @return 0 on success, otherwise error code.
 */
WAOLACOD_API int waolasc_discover_async(waolasc_t* const self);

/**
 * Tries to connect to each stored host and update all its data.
 * Notifices subscribed clients about each change with vault's callback.
 *
 * @param self scanner interface pointer.
 *
 * @return 0 on success, otherwise error code.
 */
WAOLACOD_API int waolasc_refresh(waolasc_t* const self);

/**
 * Starts asynchronous refreshing.
 *
 * @param self scanner interface pointer.
 *
 * @return 0 on success, otherwise error code.
 */
WAOLACOD_API int waolasc_refresh_async(waolasc_t* const self);

/**
 * Tries to connect to given host and update all its data.
 * Notifices subscribed clients about each change with vault's callback.
 *
 * @param self scanner interface pointer.
 * @param hw target host's model view.
 */
WAOLACOD_API void waolasc_refresh_host(waolasc_t* const self,
	const whost_view_t* hw);

/**
 * Adds client-specified host to the vault, then tries to
 * connect to it to update its data from the wire.
 * Notifices subscribed clients about each change with vault's callback.
 *
 * @param self scanner interface pointer.
 * @param hd client-provided host data.
 */
WAOLACOD_API void waolasc_add(waolasc_t* const self, const whost_data_t* const hd);

/**
 * Adds client-specified hosts to the vault, then tries to
 * connect to to each one to update its data.
 * Notifices subscribed clients about each change with vault's callback.
 *
 * @param self scanner interface pointer.
 * @param hosts array of of client-provided /ref whost_data_t.
 * @param count size of /ref hosts.
 */
WAOLACOD_API void waolasc_batch_add(waolasc_t* const self,
	whost_data_t* hosts[], size_t count);

/**
 * Deletes host from the vault.
 * Notifices subscribed clients with vault's callback.
 *
 * @param self scanner interface pointer.
 * @param hw view model of the host being deleted.
 */
WAOLACOD_API void waolasc_delete_host(waolasc_t* const self, whost_view_t* const hw);

/**
 * Cancel's currently running tasks.
 * Notifices subscribed clients with scanner's state callback.
 *
 * @param self scanner interface pointer.
 */
WAOLACOD_API void waolasc_cancel_task(waolasc_t* const self);

/**
 * Gets data of the latest operation performed by the scanner.
 *
 * @param self scanner interface pointer.
 * @param module pointer which will be set to waola module
 * performed latest operation.
 * @param op_code pointer which will be set to latest
 * operation code.
 *
 * @return pointer to the latest operation data.
 */
WAOLACOD_API const void* waolasc_get_status(const waolasc_t* const self,
	wmodule_t* module, int* op_code);

/**
 * Gets stored hosts.
 *
 * @param self scanner interface pointer.
 *
 * @return list of stored hosts.
 */
WAOLACOD_API const wcont4r_t* waolasc_get_host_list(const waolasc_t* const self);

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
WAOLACOD_API waolasc_t* waolasc_make(const wcont4r_factory cont4r_factory,
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
