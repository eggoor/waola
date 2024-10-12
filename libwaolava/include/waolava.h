/** @file waolava.h
* Waola vault public interfcae.
*
* In-memory storage for discovered hosts.
* 
* Until otherwise specified, for both passed as in-parameters
* and for returned values:
* - ownership isn't transferrend for const pointers;
* - ownership is transferrend for non-const pointers, excepting 'self' one.
*/

#pragma once

#include "waolava_merge_host_cbi.h"
#include "waolava_on_host_modified_cbi.h"
#include "waolava_callback.h"

#if __cplusplus
extern "C" {
#endif

	const wcont4r_t* waolava_get_host_view_list(const waolava_t* restrict const self);
	BOOL waolava_merge_host_cb(waolava_t* const self, whost_t* const host);
	void waolava_on_host_modified_cb(waolava_t* const self, const whost_view_t* const mhw);
	void waolava_delete_host(waolava_t* restrict const self, whost_view_t* restrict const hw);
	BOOL waolava_is_empty(const waolava_t* restrict const self);
	BOOL waolava_subscribe(const waolava_t* restrict const self,
		const waolava_subscribe_data_t* sd);
	BOOL waolava_unsubscribe(const waolava_t* restrict const self,
		const waolava_subscribe_data_t* sd);

	/**
	 * Tries to add a host specified by provided data to the vault.
	 *
	 * @param self Vault interface pointer.
	 * @param hd Host data.
	 *
	 * @return Host view interface pointer if host has been created
	 * or found in the vault (pointer's ownership remains with the vault),
	 * otherwise NULL.
	 */
	whost_view_t* waolava_add(waolava_t* restrict const self, const whost_data_t* restrict const hd);

	void waolava_batch_add(waolava_t* restrict const self, whost_data_t* restrict const hosts[], size_t count);
	const waolava_on_host_modified_cbi_t* waolava_get_host_modidied_cbi(const waolava_t* restrict const self);
	const waolava_merge_host_cbi_t* waolava_get_merge_host_cbi(const waolava_t* restrict const self);

#if __cplusplus
}   // Extern C
#endif
