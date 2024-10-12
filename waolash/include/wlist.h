/** @file list.h
* Waola linked list.
*/

#pragma once

#include "waolash_exports.h"

#if __cplusplus
extern "C" {
#endif

WAOLASHD_API wlist_t* wlist_make(void);
WAOLASHD_API void wlist_free(wlist_t* const restrict list);

WAOLASHD_API void* wlist_get_first(wlist_t* const restrict list);
WAOLASHD_API void* wlist_get_next(wlist_t* const restrict list);
WAOLASHD_API BOOL wlist_add_tail(wlist_t* list, void* const restrict value);
WAOLASHD_API BOOL wlist_is_empty(const wlist_t* const restrict list);

#if __cplusplus
}   // Extern C
#endif
