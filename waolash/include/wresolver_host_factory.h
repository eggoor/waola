/** @file wresolver_host_factory.h
 * Host resolver factory.
 */

#pragma once

#include "waolava_on_host_modified_cbi_t.h"
#include "wresolver_host_t.h"
#include "wthread_cbi_t.h"
#include "waolast_t.h"

#if __cplusplus
extern "C" {
#endif

typedef wresolver_host_t* (*wresolver_host_factory)(wthread_cbi_t* parent_cbi,
	whost_view_t* hw, const waolava_on_host_modified_cbi_t* host_modified_cbi,
	waolast_t* status);

wresolver_host_t* wmake_resolver_host(wthread_cbi_t* parent_cbi,
	whost_view_t* hw, const waolava_on_host_modified_cbi_t* host_modified_cbi,
	waolast_t* status);

int wresolver_host_free(wresolver_host_t* const self, BOOL join_thread);

#if __cplusplus
}   // Extern C
#endif
