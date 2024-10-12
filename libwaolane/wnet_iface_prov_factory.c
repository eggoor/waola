#include "pch.h"

#include "include/wnet_iface_prov.h"

wnet_iface_prov_t* wnet_iface_prov_make_impl(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory, const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory, const wnet_iface_factory net_iface_factory,
	wthread_counter_t* tc);

wnet_iface_prov_t* wmake_net_iface_prov(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory, const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory, const wnet_iface_factory net_iface_factory,
	wthread_counter_t* tc)
{
	return wnet_iface_prov_make_impl(cont4r_factory, mutex_factory,
		thread_cbi_factory, thread_factory, net_iface_factory, tc);
}
