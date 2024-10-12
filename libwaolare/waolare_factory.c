#include "pch.h"

#include "waolare.h"

waolare_t* waolare_make_impl(wcont4r_factory cont4r_factory,
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
	const waolava_on_host_modified_cbi_t* hostModifiedCbi)
{
	return wmake_waolare(wmake_cont4r_s, wmake_mutex, wmake_event,
		wmake_thread_cbi, wmake_thread, wmake_net_iface,
		wmake_net_iface_prov, wmake_resolver_hosts, wmake_resolver_host,
		tc, status, hostModifiedCbi);
}

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
	const waolava_on_host_modified_cbi_t* hostModifiedCbi)
{
	return waolare_make_impl(cont4r_factory, mutex_factory,
		event_factory, thread_cbi_factory, thread_factory,
		net_iface_factory, net_iface_prov_factory,
		resolver_hosts_factory, resolver_host_factory,
		tc, status, hostModifiedCbi);
}
