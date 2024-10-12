/** @file wcont4r.h
* Waola container interface for dynamic linking.
*/

#pragma once

#include "wcont4r_type.h"
#include "waolash_exports.h"

#if __cplusplus
extern "C" {
#endif

WAOLASHD_API void wcont4r_free(wcont4r_t* const cont);

WAOLASHD_API BOOL wcont4r_add(wcont4r_t* cont, void* const value);
WAOLASHD_API BOOL wcont4r_is_empty(const wcont4r_t* const cont);
WAOLASHD_API void* wcont4r_get_first(const wcont4r_t* const cont);
WAOLASHD_API void* wcont4r_get_next(const wcont4r_t* const cont);
WAOLASHD_API void wcont4r_reverse(wcont4r_t* const cont);
WAOLASHD_API BOOL wcont4r_delete(wcont4r_t* cont, const void* const value);

#if __cplusplus
}   // Extern C
#endif
