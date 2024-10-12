/** @file waolane.h
 * Waola network-related helpers.
 */

#pragma once

 /*
 Include in precompiled header:

 #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
 #include <Ws2tcpip.h>
 #include <iphlpapi.h>
 #else
 #include <netinet/if_ether.h>
 #endif
 */

#include "arp_cache_entry.h"
#include "wnet_iface.h"
#include "wnet_iface_prov.h"
#include "warper_cb_data.h"
