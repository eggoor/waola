/** @file wmem.h
* Waola dynamic memory routines.
*/

#pragma once

#include "waolash_exports.h"

#if __cplusplus
extern "C" {
#endif

WAOLASHD_API void* walloc(size_t n);
WAOLASHD_API void wfree(void* p);

#ifdef _DEBUG
WAOLASHD_API BOOL wheap_validate(void* p);
#endif

#if __cplusplus
}   // Extern C
#endif
