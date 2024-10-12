#include "pch.h"

#include "include/wmem.h"
#include "include/wlist.h"

WAOLASHD_API wlist_t* wlist_make(void)
{
	return wlist_make_s();
}

WAOLASHD_API void wlist_free(wlist_t* const restrict list)
{
	wlist_free_s(list);
}

WAOLASHD_API BOOL wlist_add_tail(wlist_t* list, void* const restrict value)
{
	return wlist_add_tail_s(list, value);
}

WAOLASHD_API BOOL wlist_is_empty(const wlist_t* const restrict list)
{
	return wlist_is_empty_s(list);
}

WAOLASHD_API void* wlist_get_first(wlist_t* const restrict list)
{
	return wlist_get_first_s(list);
}

WAOLASHD_API void* wlist_get_next(wlist_t* const restrict list)
{
	return wlist_get_next_s(list);
}
