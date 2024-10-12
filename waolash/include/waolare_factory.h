/** @file waolare_factory.h
* Waola host resolver factory.
*/

#pragma once

#include "waolare_t.h"
#include "wcont4r_factory.h"
#include "wmutex_factory.h"
#include "wevent_factory.h"
#include "wthread_cbi_factory.h"
#include "wthread_factory.h"
#include "wnet_iface_factory.h"
#include "wnet_iface_prov_factory.h"
#include "waolast_t.h"
#include "waolava_on_host_modified_cbi_t.h"
#include "wresolver_hosts_factory.h"
#include "wresolver_host_factory.h"

#if __cplusplus
extern "C" {
#endif

	typedef waolare_t* (*waolare_factory)(wcont4r_factory cont4r_factory,
		const wmutex_factory mutex_factory,
		const wevent_factory event_factory,
		const wthread_cbi_factory thread_cbi_factory,
		const wthread_factory thread_factory,
		const wnet_iface_factory net_iface_factory,
		const wnet_iface_prov_factory net_iface_prov_factory,
		const wresolver_hosts_factory resolver_hosts_factory,
		const wresolver_host_factory resolver_host_factory,
		wthread_counter_t* tc, waolast_t* status,
		const waolava_on_host_modified_cbi_t* hostModifiedCbi);

	waolare_t* wmake_waolare_def(wthread_counter_t* tc, waolast_t* status,
		const waolava_on_host_modified_cbi_t* hostModifiedCbi);

	waolare_t* wmake_waolare(wcont4r_factory cont4r_factory,
		const wmutex_factory mutex_factory,
		const wevent_factory event_factory,
		const wthread_cbi_factory thread_cbi_factory,
		const wthread_factory thread_factory,
		const wnet_iface_factory net_iface_factory,
		const wnet_iface_prov_factory net_iface_prov_factory,
		const wresolver_hosts_factory resolver_hosts_factory,
		const wresolver_host_factory resolver_host_factory,
		wthread_counter_t* tc, waolast_t* status,
		const waolava_on_host_modified_cbi_t* hostModifiedCbi);

	void waolare_free(waolare_t* const self);

#if __cplusplus
}   // Extern C
#endif
