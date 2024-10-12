/** @file waolare_fact4s.h
* Waola host resolver factories declarations.
*/

#pragma once

//#include "include/waolare_fact4s.h"

#if __cplusplus
extern "C" {
#endif

typedef struct waolare_fact4s
{
	wcont4r_factory make_cont4r;
	wmutex_factory make_mutex;
	wthread_cbi_factory make_thread_cbi;
	wthread_factory make_thread;
	wnet_iface_factory make_net_iface;
	wnet_iface_prov_factory make_net_iface_prov;
	wresolver_hosts_factory wmake_resolver_hosts;
	wresolver_host_factory wmake_resolver_host;
} waolare_fact4s_t;

waolare_fact4s_t* wmake_waolare_fact4s(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory,
	const wnet_iface_prov_factory net_iface_prov_factory,
	const wresolver_hosts_factory resolver_hosts_factory,
	const wresolver_host_factory resolver_host_factory);

void waolare_fact4s_free(waolare_fact4s_t* const restrict fact4s);

#if __cplusplus
}   // Extern C
#endif
