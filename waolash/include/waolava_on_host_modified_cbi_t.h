/** @file waolava_on_host_modified_cbi_t.h
* Forward declaration of Waola vault on host modified callback.
*/

#pragma once

#include "waolava_t.h"
#include "whost_view.h"

#if __cplusplus
extern "C" {
#endif

typedef void(*waolava_on_host_modified_callback)(waolava_t* const, const whost_view_t* const);

typedef struct waolava_on_host_modified_cbi waolava_on_host_modified_cbi_t;

#if __cplusplus
}   // Extern C
#endif
