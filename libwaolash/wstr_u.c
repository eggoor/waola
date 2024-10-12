#include "pch.h"

int strcpy_s(char* restrict dst, size_t dstsz, const char* restrict src)
{
	if (!dst || !src)
	{
		return EINVAL;
	}

	if (dstsz == 0 || dstsz < strlen(src) + 1) {
		return ERANGE;
	}

	while (*src) {
		*dst++ = *src++;
	}

	*dst = *src;

	return 0;
}

int strcat_s(char* restrict dst, size_t totalsz, const char* restrict src)
{
	if (!dst || !src)
	{
		return EINVAL;
	}

	size_t dstsz = strlen(dst);

	if (totalsz < dstsz + strlen(src) + 1) {
		return ERANGE;
	}

	dst += dstsz;

	while (*src) {
		*dst++ = *src++;
	}

	*dst = *src;

	return 0;
}

int sprintf_s(char* const buf, size_t size, const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	int res = vsnprintf(buf, size, fmt, argp);
	va_end(argp);
	return res;
}

int strncpy_s(char* const restrict dst, size_t dstsz,
	const char* const restrict src, size_t len)
{
	if (!dst || !src)
	{
		return EINVAL;
	}

	if (dstsz < len + 1) {
		return ERANGE;
	}

	dst[len] = '\0';
	
	while (len--) {
		dst[len] = src[len];
	}
	
	return 0;
}

const char* wstrerror_s(int err)
{
	return strerror(err);
}
