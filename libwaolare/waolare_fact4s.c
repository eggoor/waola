#include "pch.h"

#include "waolare_fact4s.h"

waolare_fact4s_t* wmake_waolare_fact4s(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory,
	const wnet_iface_prov_factory net_iface_prov_factory,
	const wresolver_hosts_factory resolver_hosts_factory,
	const wresolver_host_factory resolver_host_factory)
{
	waolare_fact4s_t* fact4s = walloc_s(sizeof(struct waolare_fact4s));

	if (!fact4s) {
		wlog_if_level(wll_warning, "No %zuB of free memory for Waolare_fact4s\n",
			sizeof(struct waolare_fact4s));
		goto end;
	}

	fact4s->make_cont4r = cont4r_factory;
	fact4s->make_mutex = mutex_factory;
	fact4s->make_thread_cbi = thread_cbi_factory;
	fact4s->make_thread = thread_factory;
	fact4s->make_net_iface = net_iface_factory;
	fact4s->make_net_iface_prov = net_iface_prov_factory;
	fact4s->wmake_resolver_hosts = resolver_hosts_factory;
	fact4s->wmake_resolver_host = resolver_host_factory;

end:
	return fact4s;
}

void waolare_fact4s_free(waolare_fact4s_t* const restrict fact4s)
{
	wfree_s(fact4s);
}
