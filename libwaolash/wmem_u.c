#include "pch.h"

void* walloc_s(size_t bytes)
{
	return malloc(bytes);
}

void wfree_s(void* const p)
{
	free(p);
}

void* wrealloc_s(void* const p, size_t bytes)
{
	return realloc(p, bytes);
}
