/** @file wresolver_hosts_factory.h
* Factory to make resolver of host list.
*/

#pragma once

#include "wresolver_hosts_t.h"

#if __cplusplus
extern "C" {
#endif

typedef wresolver_hosts_t* (*wresolver_hosts_factory)(const waolare_t* const parent);

wresolver_hosts_t* wmake_resolver_hosts(const waolare_t* const parent);
int wresolver_hosts_free(wresolver_hosts_t* const self);

#if __cplusplus
}   // Extern C
#endif
