#include "pch.h"

#include "include/wmem_s.h"
#include "include/wstr_s.h"

char* wstrdup_s(const char* const restrict src)
{
	char* dup = NULL;

	if (!wstr_is_empty_s(src)) {
		size_t buf_size = strlen(src) + 1;
		dup = walloc_s(buf_size);
		if (dup) {
			strcpy_s(dup, buf_size, src);
		}
		else {
			wlog_if_level(wll_warning, "%s: no %zuB of free memory for string\n",
				__func__, buf_size);
		}
	}

	return dup;
}

char* wstrcat_s(char* const restrict dst, const char* const restrict src)
{
	char* res = NULL;
	if (!wstr_is_empty_s(src)) {
		size_t buf_size = strlen(src) + 1;
		if (dst) {
			buf_size += strlen(dst);
		}

		res = wrealloc_s(dst, buf_size);
		if (!res) {
			wlog_if_level(wll_warning, "%s: no %zuB of free memory for string\n",
				__func__, buf_size);
			goto end;	
		}

		if (dst) {
			strcat_s(res, buf_size, src);
		}
		else {
			strcpy_s(res, buf_size, src);
		}
	}
	
end:
	return res;
}

BOOL wstr_is_empty_s(const char* const restrict str)
{
	return !str || !*str;
}

int wstrcmp_s(const char* const restrict l, const char* const restrict r)
{
	int rc = 0;

	if (l != r) {
		if (l && r) {
			rc = strcmp(l, r);
		}
		else
		{
			rc = l > r ? 1 : -1;
		}
	}

	return rc;
}

int wstrncpy_s(char* const restrict dst, size_t dstsz,
	const char* const restrict src, size_t len)
{
	return strncpy_s(dst, dstsz, src, len);
}
