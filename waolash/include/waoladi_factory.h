/** @file waoladi_factory.h
* Waola discoverer factory.
*/

#pragma once

#include "waoladi_t.h"
#include "wevent_factory.h"
#include "wnet_iface_factory.h"
#include "wnet_iface_prov_factory.h"
#include "wdiscoverer_net_factory.h"
#include "wdiscoverer_iface_factory.h"
#include "wdiscoverer_iface_monitor_factory.h"
#include "wdiscoverer_host_factory.h"

#if __cplusplus
extern "C" {
#endif

	typedef waoladi_t* (*waoladi_factory)(const wcont4r_factory cont4r_factory,
		const wmutex_factory mutex_factory,
		const wevent_factory event_factory,
		const wthread_cbi_factory thread_cbi_factory,
		const wthread_factory thread_factory,
		const wnet_iface_factory net_iface_factory,
		const wnet_iface_prov_factory net_iface_prov_factory,
		const whost_factory host_factory,
		const wdiscoverer_net_factory discoverer_net_factory,
		const wdiscoverer_iface_factory discoverer_iface_factory,
		const wdiscoverer_iface_monitor_factory discoverer_iface_monitor_factory,
		const wdiscoverer_host_factory discoverer_host_factory,
		wthread_counter_t* tc, waolast_t* status,
		const waolava_merge_host_cbi_t* mergeHostCbi);

	waoladi_t* wmake_waoladi_def(wthread_counter_t* tc, waolast_t* status,
		const waolava_merge_host_cbi_t* mergeHostCbi);

	waoladi_t* wmake_waoladi(const wcont4r_factory cont4r_factory,
		const wmutex_factory mutex_factory,
		const wevent_factory event_factory,
		const wthread_cbi_factory thread_cbi_factory,
		const wthread_factory thread_factory,
		const wnet_iface_factory net_iface_factory,
		const wnet_iface_prov_factory net_iface_prov_factory,
		const whost_factory host_factory,
		const wdiscoverer_net_factory discoverer_net_factory,
		const wdiscoverer_iface_factory discoverer_iface_factory,
		const wdiscoverer_iface_monitor_factory discoverer_iface_monitor_factory,
		const wdiscoverer_host_factory discoverer_host_factory,
		wthread_counter_t* tc, waolast_t* status,
		const waolava_merge_host_cbi_t* mergeHostCbi);

	void waoladi_free(waoladi_t* const self);

#if __cplusplus
}   // Extern C
#endif
