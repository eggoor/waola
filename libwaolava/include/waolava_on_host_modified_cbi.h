/** @file waolava_on_host_modified_cbi.h
* Waola vault on host modified callback declarations.
*/

#pragma once

#include <waolava_on_host_modified_cbi_t.h>

#if __cplusplus
extern "C" {
#endif

typedef struct waolava_on_host_modified_cbi {
	waolava_t* vault;
	waolava_on_host_modified_callback cb;
} waolava_on_host_modified_cbi_t;

#if __cplusplus
}   // Extern C
#endif
