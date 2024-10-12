#include "pch.h"

#include "waoladi.h"

waoladi_t* waoladi_make_impl(const wcont4r_factory cont4r_factory,
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
	const waolava_merge_host_cbi_t* mergeHostCbi)
{
	return wmake_waoladi(wmake_cont4r_s, wmake_mutex, wmake_event,
		wmake_thread_cbi, wmake_thread, wmake_net_iface,
		wmake_net_iface_prov, wmake_host, wmake_discoverer_net,
		wmake_discoverer_iface, wmake_discoverer_iface_monitor,
		wmake_discoverer_host, tc, status, mergeHostCbi);
}

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
	const waolava_merge_host_cbi_t* mergeHostCbi)
{
	return waoladi_make_impl(cont4r_factory, mutex_factory, event_factory, thread_cbi_factory,
		thread_factory, net_iface_factory, net_iface_prov_factory, host_factory,
		discoverer_net_factory, discoverer_iface_factory, discoverer_iface_monitor_factory,
		discoverer_host_factory, tc, status, mergeHostCbi);
}
