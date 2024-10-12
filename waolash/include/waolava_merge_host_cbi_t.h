/** @file waolava_merge_host_cbi_t.h
* Forward declaration of Waola vault merge host callback.
*/

#pragma once

#include "waolava_t.h"
#include "whost.h"

#if __cplusplus
extern "C" {
#endif

typedef BOOL(*waolava_merge_host_callback)(waolava_t* const, whost_t* const);

typedef struct waolava_merge_host_cbi waolava_merge_host_cbi_t;

#if __cplusplus
}   // Extern C
#endif
