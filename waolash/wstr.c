#include "pch.h"

#include "include/waolash_exports.h"

WAOLASHD_API char* wstrdup(const char* const restrict src)
{
	return wstrdup_s(src);
}

WAOLASHD_API char* wstrcat(char* dst, const char* const restrict src)
{
	return wstrcat_s(dst, src);
}

WAOLASHD_API BOOL wstr_is_empty(const char* const restrict str)
{
	return wstr_is_empty_s(str);
}

WAOLASHD_API int wstrcpy_s(char* const restrict dst, size_t dstsz,
	const char* const restrict src)
{
	return strcpy_s(dst, dstsz, src);
}
