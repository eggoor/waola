/** @file wstr.h
* Waola string routines.
*/

#pragma once

#include "waolash_exports.h"

#if __cplusplus
extern "C" {
#endif

WAOLASHD_API BOOL wstr_is_empty(const char* const str);
WAOLASHD_API char* wstrdup(const char* const src);
WAOLASHD_API char* wstrcat(char* dst, const char* const src);
WAOLASHD_API int wstrcpy_s(char* dst, size_t dstsz, const char* const src);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
WAOLASHD_API char* wstr_16_to_8(const wchar_t* const wstr);
WAOLASHD_API wchar_t* wstr_8_to_16(const char* const str);
#endif

#if __cplusplus
}   // Extern C
#endif
