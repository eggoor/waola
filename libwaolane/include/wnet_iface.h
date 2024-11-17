/** @file wnet_iface.h
* Waola network interface wrapper.
*/

#pragma once

/*
Include in precompiled header:

#include <stdint.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
 #include <Ws2tcpip.h>
 #include <iphlpapi.h>
#else
#include <netinet/in.h>
#endif
*/

#include "wnet_iface_t.h"
#include "warper_cb_data_t.h"

#if __cplusplus
extern "C" {
#endif

void wnet_iface_free(wnet_iface_t* self);

int wnet_iface_get_index(const wnet_iface_t* self);
const char* wnet_iface_get_name(const wnet_iface_t* self);
sa_family_t wnet_iface_get_addr_family(const wnet_iface_t* self);
struct sockaddr* wnet_iface_get_sockaddr(const wnet_iface_t* self);
size_t wnet_iface_get_sockaddr_len(const wnet_iface_t* self);
uint32_t wnet_iface_get_self_ip_addr(const wnet_iface_t* self);
uint32_t wnet_iface_get_subnet_mask(const wnet_iface_t* self);
const byte* wnet_iface_get_mac_addr(const wnet_iface_t* self);
BOOL wnet_iface_is_ip_in_subnet(const wnet_iface_t* self, uint32_t ip_addr);
BOOL wnet_iface_query_arp(const wnet_iface_t* self, uint32_t ip_addr);
void wnet_iface_set_requesting_finished(const wnet_iface_t* self);
int wnet_iface_prepare_for_arp(wnet_iface_t* self);
int wnet_iface_start_listening(wnet_iface_t* self,
	warper_cb_data_t* arp_cbd);
void wnet_iface_wait_for_warper_listening_finished(const wnet_iface_t* self,
	BOOL join_thread);
BOOL wnet_iface_is_ready_for_arp(const wnet_iface_t* self);
int wnet_iface_get_errno(const wnet_iface_t* self);
int wnet_iface_resolve_ip_2_mac(const wnet_iface_t* self,
	uint32_t target_ip_addr, byte* mac_addr);

#if __cplusplus
}   // Extern C
#endif
