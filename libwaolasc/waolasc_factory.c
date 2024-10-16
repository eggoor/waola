/** @file waolasc_factory.h
* Waola scanner factory implementation.
*/

#include "pch.h"

waolasc_t* waolasc_make_impl(const wcont4r_factory cont4r_factory,
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
	const waolava_subscribe_data_t* vault_cbi)
{
	return waolasc_make_impl(cont4r_factory, mutex_factory, event_factory,
		thread_counter_factory, thread_cbi_factory, thread_factory,
		net_iface_factory, net_iface_prov_factory, host_factory,
		host_from_data_factory, host_view_factory, waolava_factory,
		waolava_on_host_modified_cbi_factory,
		waolava_merge_host_cbi_factory, waoladi_factory,
		discoverer_net_factory, discoverer_iface_factory,
		discoverer_iface_monitor_factory, discoverer_host_factory,
		waolare_factory, resolver_hosts_factory, resolver_host_factory,
		status_factory, state_cbi, vault_cbi);
}

waolasc_t* wmake_waolasc_def_s(const waolasc_subscribe_data_t* state_cbi,
	const waolava_subscribe_data_t* vault_cbi)
{
	return waolasc_make_s(wmake_cont4r_s, wmake_mutex, wmake_event,
		wmake_thread_counter, wmake_thread_cbi, wmake_thread,
		wmake_net_iface, wmake_net_iface_prov,
		wmake_host, wmake_host_from_data, wmake_host_view,
		wmake_waolava, wmake_waolava_on_host_modified_cbi,
		wmake_waolava_merge_host_cbi, wmake_waoladi,
		wmake_discoverer_net, wmake_discoverer_iface,
		wmake_discoverer_iface_monitor, wmake_discoverer_host,
		wmake_waolare, wmake_resolver_hosts, wmake_resolver_host, wmake_waolast,
		state_cbi, vault_cbi);
}
