/** @file wdiscoverer_net_factory.h
*
* Waola network discoverer factory.
*/

#pragma once

#include "waoladi_t.h"

#if __cplusplus
extern "C" {
#endif

typedef struct wdiscoverer_net wdiscoverer_net_t;

typedef wdiscoverer_net_t* (*wdiscoverer_net_factory)(waoladi_t* const di);

wdiscoverer_net_t* wmake_discoverer_net(waoladi_t* const di);

#if __cplusplus
}   // Extern C
#endif
