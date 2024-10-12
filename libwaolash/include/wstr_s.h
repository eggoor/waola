#pragma once

#if __cplusplus
extern "C" {
#endif

BOOL wstr_is_empty_s(const char* const restrict str);
char* wstrdup_s(const char* const restrict src);
char* wstrcat_s(char* const restrict dst, const char* const restrict src);
int wstrncpy_s(char* const restrict dst, size_t buf_size,
	const char* const restrict src, size_t len);
int wstrcmp_s(const char* const restrict l, const char* const restrict r);
const char* wstrerror_s(int err);

#if __cplusplus
}   // Extern C
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "wstr_w.h"
#else
#include "wstr_u.h"
#endif
