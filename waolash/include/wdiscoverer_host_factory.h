/** @file wdiscoverer_host_factory.h
*
* Waola host discoverer factory.
*/

#pragma once
/*
Include in precompiled header:
#include <stdint.h>
*/

#include <wnet_iface_t.h>
#include <waolast_t.h>
#include <wthread_cbi_t.h>

#if __cplusplus
extern "C" {
#endif

typedef struct wdiscoverer_host wdiscoverer_host_t;

typedef wdiscoverer_host_t* (*wdiscoverer_host_factory)(const waoladi_fact4s_t* fact4s,
	wnet_iface_t* nif, uint32_t target_ip_addr, const byte* mac_addr,
	waolast_t* status, wthread_cbi_t* master_cbi);

wdiscoverer_host_t* wmake_discoverer_host(const waoladi_fact4s_t* fact4s,
	wnet_iface_t* nif, uint32_t target_ip_addr, const byte* mac_addr,
	waolast_t* status, wthread_cbi_t* master_cbi);

#if __cplusplus
}   // Extern C
#endif
