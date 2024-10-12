#pragma once

#if __cplusplus
extern "C" {
#endif

struct waoladi_fact4s
{
	wcont4r_factory make_cont4r;
	wmutex_factory make_mutex;
	wevent_factory make_event;
	wthread_cbi_factory make_thread_cbi;
	wthread_factory make_thread;
	wnet_iface_factory make_net_iface;
	wnet_iface_prov_factory make_net_iface_prov;
	whost_factory make_host;
	wdiscoverer_net_factory make_discoverer_net;
	wdiscoverer_iface_factory make_discoverer_iface;
	wdiscoverer_host_factory make_discoverer_host;
	wdiscoverer_iface_monitor_factory make_discoverer_iface_monitor;
};

waoladi_fact4s_t* waoladi_fact4s_make(const wcont4r_factory cont4r_factory,
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
	const wdiscoverer_host_factory discoverer_host_factory);

void waoladi_fact4s_free(waoladi_fact4s_t* const restrict fact4s);

#if __cplusplus
}   // Extern C
#endif
