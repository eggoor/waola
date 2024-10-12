#include "pch.h"

void* walloc_s(size_t bytesCount)
{
	return HeapAlloc(GetProcessHeap(), 0, bytesCount);
}

void* wrealloc_s(void* p, size_t bytesCount)
{
	return HeapReAlloc(GetProcessHeap(), 0, p, bytesCount);
}

void wfree_s(void* p)
{
	HeapFree(GetProcessHeap(), 0, p);
}

#ifdef _DEBUG

BOOL wheap_validate_s(void* p)
{
	BOOL is_valid = HeapValidate(GetProcessHeap(), 0, p);
	return is_valid;
}

#endif