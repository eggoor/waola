/** @file wnet_iface_prov_factory.h
 * Waola Waola network interface provider factory.
 */

#pragma once

#include "wnet_iface_factory.h"
#include "wnet_iface_prov_t.h"

#if __cplusplus
extern "C" {
#endif

/**
 * Pointer to network interface provider factory function.
 *
 * @param cont4r_factory Container factory.
 * @param net_iface_factory Network interface factory.
 *
 * @return network interfcae provider pointer.
 */
typedef wnet_iface_prov_t* (*wnet_iface_prov_factory)(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory, const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory, const wnet_iface_factory net_iface_factory,
	wthread_counter_t* tc);

wnet_iface_prov_t* wmake_net_iface_prov(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory, const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory, const wnet_iface_factory net_iface_factory,
	wthread_counter_t* tc);

#if __cplusplus
}   // Extern C
#endif
