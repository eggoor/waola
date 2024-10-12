/** @file arp_cache_entry.h
* IP and MAC addresses from ARP cache.
*/

#pragma once

/*
Include in precompiled header:

#include <net/ethernet.h>
*/

#if __cplusplus
extern "C" {
#endif

typedef struct arp_cache_entry {
	uint32_t ipAddr;
	byte macAddr[ETHER_ADDR_LEN];
} arp_cache_entry_t;

#if __cplusplus
}   // Extern C
#endif
