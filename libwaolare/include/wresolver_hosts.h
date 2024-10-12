/** @file wresolver_hosts.h
* Waola hosts resolver.
*
* Resolves hostnames, IP and MAC addresses
* of a given host list.
*/

#pragma once

#include "waolare_t.h"

#if __cplusplus
extern "C" {
#endif

int wresolver_hosts_resolve(wresolver_hosts_t* self,
	const wcont4r_t* host_list);
void wresolver_hosts_cancel(wresolver_hosts_t* const restrict hsr);

BOOL wresolver_hosts_increment_thread_count(wresolver_hosts_t* self);
void wresolver_hosts_decrement_thread_count(wresolver_hosts_t* self);

BOOL wresolver_hosts_is_cancelled(const wresolver_hosts_t* self);
wnet_iface_prov_t* wresolver_hosts_get_iface_prov(const wresolver_hosts_t* self);

#if __cplusplus
}   // Extern C
#endif
