/** @file waolane_fact4s.h
* Waola ARP client factories declarations.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef struct waolane_fact4s
{
	wcont4r_factory make_cont4r;
	wmutex_factory make_mutex;
	wthread_cbi_factory make_thread_cbi;
	wthread_factory make_thread;
	wnet_iface_factory make_net_iface;
} waolane_fact4s_t;

waolane_fact4s_t* wmake_waolane_fact4s(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory);

void waolane_fact4s_free(waolane_fact4s_t* const restrict fact4s);

#if __cplusplus
}   // Extern C
#endif
