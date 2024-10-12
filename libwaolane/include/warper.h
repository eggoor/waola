/** @file warper.h
* Waola ARP client.
*
* Resolves MAC addresses via Address Resolution Protocol.
*/

#pragma once

#include "wnet_iface.h"
#include "warper_cb_data.h"

#if __cplusplus
extern "C" {
#endif

/**
 * \struct waola ARP client.
 */
typedef struct warper warper_t;
int warper_resolve_ip_2_mac(uint32_t self_ip_addr, uint32_t target_ip, byte* const restrict mac_addr);

#if __cplusplus
}   // Extern C
#endif
