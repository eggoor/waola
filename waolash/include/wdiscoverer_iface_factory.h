/** @file wdiscoverer_iface_factory.h
*
* Waola inteface discoverer factory.
*/

#pragma once

#include "wdiscoverer_net_factory.h"
#include "wnet_iface_t.h"
#include "waolava_merge_host_cbi_t.h"

#if __cplusplus
extern "C" {
#endif

	typedef struct wdiscoverer_iface wdiscoverer_iface_t;

	typedef wdiscoverer_iface_t* (*wdiscoverer_iface_factory)(wthread_cbi_t* parent_cbi,
		wnet_iface_t* nif, const waolava_merge_host_cbi_t* const mh_cbi);

	wdiscoverer_iface_t* wmake_discoverer_iface(wthread_cbi_t* parent_cbi,
		wnet_iface_t* nif, const waolava_merge_host_cbi_t* const mh_cbi);

#if __cplusplus
}   // Extern C
#endif
