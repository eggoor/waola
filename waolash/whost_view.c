#include "pch.h"

#include "include/whost_view.h"

WAOLASHD_API const char* whost_view_get_display_name(const whost_view_t* const restrict hw)
{
	return whost_view_get_display_name_s(hw);
}

WAOLASHD_API const char* whost_view_get_exact_display_name(const whost_view_t* const restrict hw)
{
	return whost_view_get_exact_display_name_s(hw);
}

WAOLASHD_API const char* whost_view_get_hostname(const whost_view_t* const restrict hw)
{
	return whost_view_get_hostname_s(hw);
}

WAOLASHD_API const char* whost_view_get_ip_addr(const whost_view_t* const restrict hw)
{
	return whost_view_get_ip_addr_s(hw);
}

WAOLASHD_API const char* whost_view_get_mac_addr(const whost_view_t* const restrict hw)
{
	return whost_view_get_mac_addr_s(hw);
}

WAOLASHD_API const char* whost_view_get_last_seen_online(const whost_view_t* const restrict hw)
{
	return whost_view_get_last_seen_online_s(hw);
}

WAOLASHD_API const char* whost_view_get_op_result(const whost_view_t* const restrict hw)
{
	return whost_view_get_op_result_s(hw);
}

WAOLASHD_API const whost_t* whost_view_get_host(const whost_view_t* const restrict hw)
{
	return whost_view_get_host_s(hw);
}

WAOLASHD_API void* whost_view_get_extra_data(const whost_view_t* const restrict hw)
{
	return whost_view_get_extra_data_s(hw);
}

WAOLASHD_API whost_changed_field_t whost_view_get_state(const whost_view_t* const restrict hw)
{
	return whost_view_get_state_s(hw);
}

WAOLASHD_API BOOL whost_view_is_field_updated(const whost_view_t* hw,
	whost_field_t hf)
{
	return whost_view_is_field_updated_s(hw, hf);
}

WAOLASHD_API const char* whost_view_get_host_id(const whost_view_t* const restrict hw)
{
	return whost_view_get_host_id_s(hw);
}

WAOLASHD_API char* whost_view_to_string(const whost_view_t* const restrict hw)
{
	return whost_view_to_string_s(hw);
}


WAOLASHD_API int whost_view_compare(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_view_compare_s(l, r);
}

WAOLASHD_API int whost_view_compare_display_name(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_view_compare_display_name_s(l, r);
}

WAOLASHD_API int whost_view_compare_host_name(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_view_compare_host_name_s(l, r);
}

WAOLASHD_API int whost_view_compare_ip_addr(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_view_compare_ip_addr_s(l, r);
}

WAOLASHD_API int whost_view_compare_mac_addr(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_view_compare_mac_addr_s(l, r);
}

WAOLASHD_API int whost_view_compare_last_seen_online(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_view_compare_last_seen_online_s(l, r);
}

WAOLASHD_API int whost_view_compare_op_result(const whost_view_t* l,
	const whost_view_t* r)
{
	return whost_view_compare_op_result_s(l, r);
}


WAOLASHD_API BOOL whost_view_update_host(whost_view_t* hw, const whost_t* const restrict host)
{
	return whost_view_update_host_s(hw, host);
}

WAOLASHD_API BOOL whost_view_set_display_name(whost_view_t* hw,
	const char* display_name)
{
	return whost_view_set_display_name_s(hw, display_name);
}

WAOLASHD_API BOOL whost_view_set_hostname(whost_view_t* hw, const char* const restrict hostname)
{
	return whost_view_set_hostname_s(hw, hostname);
}

WAOLASHD_API BOOL whost_view_set_ip_addr(whost_view_t* hw, const char* const restrict ip_addr_str)
{
	return whost_view_set_ip_addr_s(hw, ip_addr_str);
}

WAOLASHD_API BOOL whost_view_set_mac_addr(whost_view_t* hw, const byte* const restrict mac_addr)
{
	return whost_view_set_mac_addr_s(hw, mac_addr);
}

WAOLASHD_API BOOL whost_view_set_mac_addr_string(whost_view_t* hw, const char* const restrict mac_addr_str)
{
	return whost_view_set_mac_addr_string_s(hw, mac_addr_str);
}

WAOLASHD_API void whost_view_set_op_result(whost_view_t* const restrict hw, whost_op_result_t or)
{
	whost_view_set_op_result_s(hw, or);
}

WAOLASHD_API void whost_view_set_extra_data(whost_view_t* hw,
	const void* data, size_t size)
{
	whost_view_set_extra_data_s(hw, data, size);
}

WAOLASHD_API void whost_view_set_field_change_processed(const whost_view_t* hw,
	whost_changed_field_t cf)
{
	whost_view_set_field_change_processed_s(hw, cf);
}

WAOLASHD_API whost_changed_field_t whost_field_2_changed_field(whost_field_t hf)
{
	return whost_field_2_changed_field_s(hf);
}
