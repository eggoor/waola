/** @file wnet_iface_prov.h
* Waola network interface provider.
*/

#pragma once

#include "wnet_iface.h"

#if __cplusplus
extern "C" {
#endif

/**
 * \struct waola network interface provider.
 */
typedef struct wnet_iface_prov wnet_iface_prov_t;

void wnet_iface_prov_free(wnet_iface_prov_t* self);

const wcont4r_t* wnet_iface_prov_get_collected_ifaces(wnet_iface_prov_t* self);
wnet_iface_t* wnet_iface_prov_make_dummy(const wnet_iface_prov_t* self);
void (*wnet_iface_prov_get_canceller(wnet_iface_prov_t* self))(wnet_iface_prov_t* self);
uint32_t wnet_iface_prov_lookup_ip_addr_in_arp_cache(const wnet_iface_prov_t* self,
	const byte* const restrict mac_addr);
BOOL wnet_iface_prov_prepare_for_arp(wnet_iface_prov_t* self);
wnet_iface_t* wnet_iface_prov_find_iface(wnet_iface_prov_t* self, uint32_t ip_addr);
const wcont4r_t* wnet_iface_prov_get_cached(const wnet_iface_prov_t* self);
void wnet_iface_prov_wait_for_listening_finished(const wnet_iface_prov_t* self);

#if __cplusplus
}   // Extern C
#endif
