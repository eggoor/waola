#include "pch.h"

#include "include/wmem.h"

WAOLASHD_API void* walloc(size_t n)
{
	return walloc_s(n);
}

WAOLASHD_API void wfree(void* p)
{
	wfree_s(p);
}

#ifdef _DEBUG

WAOLASHD_API BOOL wheap_validate(void* p)
{
	return wheap_validate_s(p);
}

#endif
