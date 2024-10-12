#include "pch.h"

#include "include/wcont4r.h"

WAOLASHD_API void wcont4r_free(wcont4r_t* const restrict cont)
{
	wcont4r_free_s(cont);
}

WAOLASHD_API BOOL wcont4r_add(wcont4r_t* cont, void* const restrict value)
{
	return wcont4r_add_s(cont, value);
}

WAOLASHD_API BOOL wcont4r_is_empty(const wcont4r_t* const restrict cont)
{
	return wcont4r_is_empty_s(cont);
}

WAOLASHD_API void* wcont4r_get_first(const wcont4r_t* const restrict cont)
{
	return wcont4r_get_first_s(cont);
}

WAOLASHD_API void* wcont4r_get_next(const wcont4r_t* const restrict cont)
{
	return wcont4r_get_next_s(cont);
}

WAOLASHD_API void wcont4r_reverse(wcont4r_t* const restrict cont)
{
	wcont4r_reverse_s(cont);
}

WAOLASHD_API BOOL wcont4r_delete(wcont4r_t* cont, const void* const restrict value)
{
	return wcont4r_delete_s(cont, value);
}
