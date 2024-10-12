#include "pch.h"

void* ealloc(size_t bc)
{
	return HeapAlloc(GetProcessHeap(), 0, bc);
}

void efree(void* p)
{
	HeapFree(GetProcessHeap(), 0, p);
}
