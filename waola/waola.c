#include "pch.h"

#include "include/waola.h"


WAOLAD_API void waola_set_log_level(wlog_level_t level)
{
	wlog_set_level(level);
}

WAOLAD_API int waolam(wcont4r_factory cont4r_factory, const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory, const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory, const wnet_iface_prov_factory net_iface_prov_factory,
	const wthread_counter_factory thread_counter_factory, const char* const mac_addresses[], size_t count)
{
	return waola_s(cont4r_factory, mutex_factory, thread_cbi_factory,
		thread_factory, net_iface_factory, net_iface_prov_factory,
		thread_counter_factory, mac_addresses, count);
}

WAOLAD_API int waolah(whost_view_t* const hosts[], size_t count)
{
	return waolah_s(wmake_cont4r_s, wmake_mutex, wmake_thread_cbi,
		wmake_thread, wmake_net_iface, wmake_net_iface_prov,
		wmake_thread_counter, hosts, count);
}
