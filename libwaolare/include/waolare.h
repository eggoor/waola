/** @file waolare.h
* Waola host resolver library.
*
* Resolves hostnames, IP and MAC addresses.
*/

#pragma once

/*
Include in precompiled header:

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WS2tcpip.h>
*/

#include "waolare_t.h"
#include "wresolver_host_factory.h"
#include "wresolver_hosts_factory.h"
#include "waolare_factory.h"
#include "waolare_opcode.h"
#include "wresolver_hostname.h"

#if __cplusplus
extern "C" {
#endif

	/**
	 * Makes resolver using default factories.
	 *
	 * @param tc Thread counter which limits max
	 * thread count used by resolver.
	 * @param status Status reporter.
	 * @param hostModifiedCbi Vault host modified callback.
	 *
	 * @return resolver interface pointer.
	 */
	waolare_t* wmake_waolare_def(wthread_counter_t* tc, waolast_t* status,
		const waolava_on_host_modified_cbi_t* hostModifiedCbi);

	/**
	 * Frees resolver.
	 *
	 * @param self resolver interface pointer.
	 */
	void waolare_free(waolare_t* const self);

	int waolare_refresh(waolare_t* self, const wcont4r_t* host_list,
		const async_operation_data_t* const restrict async_cbi);
	void waolare_refresh_host(waolare_t* self, const whost_view_t* const restrict hw);
	void waolare_cancel_task(waolare_t* self);

#if __cplusplus
}   // Extern C
#endif
