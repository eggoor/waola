/** @file libwaolasc.h
* Waola network scanner library.
*
* Discovers, resolves and stores network hosts
* in the in-memory vault.
*/

#pragma once

/*
Include in precompiled header:

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WS2tcpip.h>
#endif
*/

#include <waolasc_t.h>
#include <waolasc_opcode.h>
#include <waolasc_callback.h>

#if __cplusplus
extern "C" {
#endif

/**
 * Frees scanner.
 *
 * @param self scanner interface pointer.
 */
void waolasc_free_s(waolasc_t* self);

/**
 * Subscribes for scanner's state changes.
 *
 * @param self scanner interface pointer.
 * @param sd the subscriber object and callback which
 * wil be called each time when scanner state is changed.
 *
 * @return TRUE if the caller has successfully subscribed,
 * FALSE if callback was already registered.
 */
BOOL waolasc_subscribe_state_s(waolasc_t* self,
	const waolasc_subscribe_data_t* sd);

/**
 * Unsubscribes from scanner's state changes.
 *
 * @param self scanner interface pointer.
 * @param sd callback to unsubscribe.
 *
 * @return TRUE if the caller has successfully subscribed,
 * FALSE if callback wasn't registered.
 */
BOOL waolasc_unsubscribe_state_s(waolasc_t* self,
	const waolasc_subscribe_data_t* sd);

/**
 * Subscribes for vault operations.
 *
 * @param self scanner interface pointer.
 * @param sd caller and callback which will be called for
*	each operation performed on the vault.
 *
 * @return TRUE if the caller has successfully subscribed,
 * FALSE if callback was already registered.
 */
BOOL waolasc_subscribe_vault_s(waolasc_t* self,
	const waolava_subscribe_data_t* sd);

/**
 * Unsubscribes from vault's operations.
 *
 * @param self scanner interface pointer.
 * @param sd callback to unsubscribe.
 *
 * @return TRUE if the caller has successfully unsubscribed,
 * FALSE if callback wasn't registered.
 */
BOOL waolasc_unsubscribe_vault_s(waolasc_t* self,
	const waolava_subscribe_data_t* sd);

/**
 * Checks if there is any discovered host.
 *
 * @param self scanner interface pointer.
 *
 * @return TRUE if there is no any discovered host.
 */
BOOL waolasc_is_empty_s(const waolasc_t* self);

/**
 * Gets currently running task flag(s).
 *
 * @param self scanner interface pointer.
 *
 * @return currently running task flag(s).
 */
wtask_t waolasc_get_current_tasks_s(const waolasc_t* self);

/**
 * Sets scanner's maximum thread count.
 *
 * @param self scanner interface pointer.
 * @param max_threads thread count limit.
 */
void waolasc_set_max_threads_s(waolasc_t* self, uint32_t max_threads);

/**
 * Discovers all possible hosts against all active network interfaces.
 * Notifices subscribed clients about each discovered host
 * with vault's callback.
 *
 * @param self scanner interface pointer.
 *
 * @return 0 on success, otherwise exit code.
 */
int waolasc_discover_s(waolasc_t* self);

/**
 * Starts asynchronous discovery.
 *
 * @param self scanner interface pointer.
 *
 * @return 0 on success, otherwise exit code.
 */
int waolasc_discover_async_s(waolasc_t* self);

/**
 * Tries to connect to each stored host and update all its data.
 * Notifices subscribed clients about each change with vault's callback.
 *
 * @param self scanner interface pointer.
 *
 * @return 0 on success, otherwise error code.
 */
int waolasc_refresh_s(waolasc_t* self);

/**
 * Starts asynchronous refreshing.
 *
 * @param self scanner interface pointer.
 *
 * @return 0 on success, otherwise error code.
 */
int waolasc_refresh_async_s(waolasc_t* self);

/**
 * Tries to connect to given host and update all its data.
 * Notifices subscribed clients about each change with vault's callback.
 *
 * @param self scanner interface pointer.
 * @param hw target host's model view.
 */
void waolasc_refresh_host_s(waolasc_t* self, const whost_view_t* const restrict hw);

/**
 * Adds client-specified host to the vault,
 * then tries to connect to it to update its data.
 *
 * Notifices subscribed clients about each change with vault's callback.
 *
 * @param self scanner interface pointer.
 * @param hd client-specified host data.
 */
void waolasc_add_s(waolasc_t* self, const whost_data_t* const restrict hd);

/**
 * Adds client-specified hosts to the vault, then tries to
 * connect to to each one to update its data.
 * 
 * Notifices subscribed clients about each change with vault's callback.
 *
 * @param self scanner interface pointer.
 * @param hosts array of of client-provided /ref whost_data_t.
 * @param count size of /ref hosts.
 */
void waolasc_batch_add_s(waolasc_t* self,
	whost_data_t* hosts[], size_t count);

/**
 * Deletes host from the vault.
 * Notifices subscribed clients with vault's callback.
 *
 * @param self scanner interface pointer.
 * @param hw view model of the host being deleted.
 */
void waolasc_delete_host_s(waolasc_t* self, whost_view_t* const restrict hw);

/**
 * Cancel's currently running tasks.
 * Notifices subscribed clients with scanner's state callback.
 *
 * @param self scanner interface pointer.
 */
void waolasc_cancel_task_s(waolasc_t* self);

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
const void* waolasc_get_status_s(const waolasc_t* self,
	wmodule_t* module, int* op_code);

/**
 * Gets stored hosts.
 *
 * @param self scanner interface pointer.
 *
 * @return list of stored hosts.
 */
const wcont4r_t* waolasc_get_host_list_s(const waolasc_t* self);

#if __cplusplus
}   // Extern C
#endif
