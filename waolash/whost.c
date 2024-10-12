#include "pch.h"

#include "include/whost.h"

WAOLASHD_API const char* whost_get_display_name(const whost_t* const restrict host)
{
	return whost_get_display_name_s(host);
 }

WAOLASHD_API const char* whost_get_host_name(const whost_t* const restrict host)
{
	return whost_get_hostname_s(host);
 }

WAOLASHD_API const struct sockaddr* whost_get_sockaddr(const whost_t* const restrict host)
{
	return whost_get_sockaddr_s(host);
}

WAOLASHD_API const byte* whost_get_mac_addr(const whost_t* const restrict host)
{
	return whost_get_mac_addr_s(host);
 }

WAOLASHD_API time_t whost_get_last_seen_online(const whost_t* const restrict host)
{
	return whost_get_last_seen_online_s(host);
}

WAOLASHD_API whost_changed_field_t whost_get_state(const whost_t* const restrict host)
{
	return whost_get_state_s(host);
}

WAOLASHD_API int whost_get_op_result(const whost_t* const restrict host)
{
	return whost_get_op_result_s(host);
}

WAOLASHD_API int whost_compare(const whost_t* this_host,
	const whost_t* other_host)
{
	return whost_compare_s(this_host, other_host);
}
