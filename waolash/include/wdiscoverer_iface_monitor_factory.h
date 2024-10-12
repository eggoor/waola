/** @file wdiscoverer_iface_monitor_factory.h
*
* Waola interface discoverer monitor factory.
*/

#pragma once

#include "waoladi_fact4s_t.h"

#if __cplusplus
extern "C" {
#endif

typedef struct wdiscoverer_iface_monitor wdiscoverer_iface_monitor_t;

typedef wdiscoverer_iface_monitor_t* (*wdiscoverer_iface_monitor_factory)(const waoladi_fact4s_t* fact4s,
	wthread_cbi_t* parent_cbi);

wdiscoverer_iface_monitor_t* wmake_discoverer_iface_monitor(const waoladi_fact4s_t* fact4s,
	wthread_cbi_t* parent_cbi);

void wdiscoverer_iface_monitor_free(wdiscoverer_iface_monitor_t* const drim);

#if __cplusplus
}   // Extern C
#endif
