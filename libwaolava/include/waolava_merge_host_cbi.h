/** @file waolava_merge_host_cbi.h
* Waola vault merge host callback declarations.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef BOOL(*waolava_merge_host_callback)(waolava_t* const, whost_t* const);

typedef struct waolava_merge_host_cbi {
	waolava_t* vault;
	waolava_merge_host_callback cb;
} waolava_merge_host_cbi_t;

#if __cplusplus
}   // Extern C
#endif
