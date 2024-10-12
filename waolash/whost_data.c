#include "pch.h"

#include "include/whost_data.h"

WAOLASHD_API whost_data_t* whost_data_make(const char* display_name,
	const char* hostname, const char* ip_addr,
	const char* mac_addr, time_t lso)
{
	return whost_data_make_s(display_name,
		hostname, ip_addr, mac_addr, lso);
}

WAOLASHD_API void whost_data_free(whost_data_t* const restrict hd)
{
	whost_data_free_s(hd);
}
