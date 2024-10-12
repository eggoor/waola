#include "pch.h"

static bool IsNullOrEmpty(const char* const str)
{
	return !str || !*str;
}

char* StrDup(const char* const src)
{
	char* dup = NULL;

	if (!IsNullOrEmpty(src)) {
		size_t buf_size = strlen(src) + 1;
		wstrcpy_s(dup = new char[buf_size], buf_size, src);
	}

	return dup;
}
