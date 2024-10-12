#include "pch.h"

#include "waoladi_fact4s.h"

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
	const wdiscoverer_host_factory discoverer_host_factory)
{
	waoladi_fact4s_t* fact4s = walloc_s(sizeof(struct waoladi_fact4s));

	if (!fact4s) {
		wlog_if_level(wll_warning, "No %zuB of free memory for Waola discoverer factoriess\n",
			sizeof(struct waoladi_fact4s));
		goto end;
	}

	fact4s->make_cont4r = cont4r_factory;
	fact4s->make_mutex = mutex_factory;
	fact4s->make_event = event_factory;
	fact4s->make_thread_cbi = thread_cbi_factory;
	fact4s->make_thread = thread_factory;
	fact4s->make_net_iface = net_iface_factory;
	fact4s->make_net_iface_prov = net_iface_prov_factory;
	fact4s->make_host = host_factory;
	fact4s->make_discoverer_net = discoverer_net_factory;
	fact4s->make_discoverer_iface = discoverer_iface_factory;
	fact4s->make_discoverer_iface_monitor = discoverer_iface_monitor_factory;
	fact4s->make_discoverer_host = discoverer_host_factory;

end:
	return fact4s;
}

void waoladi_fact4s_free(waoladi_fact4s_t* const restrict fact4s)
{
	wfree_s(fact4s);
}
