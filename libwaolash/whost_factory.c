#include "pch.h"

#include <whost_factory.h>

whost_t* whost_make_impl(const char* display_name, const char* hostname,
	const struct sockaddr* sockaddr, const byte* const mac_addr, time_t lso);

whost_t* wmake_host(const char* display_name, const char* hostname,
	const struct sockaddr* sockaddr, const byte* const mac_addr, time_t lso)
{
	return whost_make_impl(display_name, hostname, sockaddr, mac_addr, lso);
}

whost_t* whost_from_data_impl(const whost_data_t* const hd);

whost_t* wmake_host_from_data(const whost_data_t* const hd)
{
	return whost_from_data_impl(hd);
}
