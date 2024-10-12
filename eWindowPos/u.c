#include "pch.h"

void* ealloc(size_t bc)
{
	return malloc(bc);
}

void efree(void* p)
{
	free(p);
}
