#pragma once

/*
Include in precompiled header:

#include <stdint.h>
*/

#include <waolash_t.h>

#include "whost_s.h"

#if __cplusplus
extern "C" {
#endif

	void whost_view_free(whost_view_t* const restrict hostView);

	const char* whost_view_get_display_name_s(const whost_view_t* const restrict hw);
	const char* whost_view_get_exact_display_name_s(const whost_view_t* const restrict hw);
	const char* whost_view_get_hostname_s(const whost_view_t* const restrict hw);
	const char* whost_view_get_ip_addr_s(const whost_view_t* const restrict hw);
	const char* whost_view_get_mac_addr_s(const whost_view_t* const restrict hw);
	const char* whost_view_get_last_seen_online_s(const whost_view_t* const restrict hw);
	const char* whost_view_get_op_result_s(const whost_view_t* const restrict hw);
	const char* whost_view_get_host_id_s(const whost_view_t* const restrict hw);
	whost_changed_field_t whost_view_get_state_s(const whost_view_t* const restrict hw);
	const whost_t* whost_view_get_host_s(const whost_view_t* const restrict hw);
	void* whost_view_get_extra_data_s(const whost_view_t* const restrict hw);
	BOOL whost_view_is_updated_s(const whost_view_t* const restrict hw);
	BOOL whost_view_is_field_updated_s(const whost_view_t* const restrict hw, whost_field_t hf);
	char* whost_view_to_string_s(const whost_view_t* const restrict hw);

	int whost_view_compare_s(const whost_view_t* l, const whost_view_t* const restrict r);
	int whost_view_compare_display_name_s(const whost_view_t* l, const whost_view_t* const restrict r);
	int whost_view_compare_host_name_s(const whost_view_t* l, const whost_view_t* const restrict r);
	int whost_view_compare_ip_addr_s(const whost_view_t* l, const whost_view_t* const restrict r);
	int whost_view_compare_mac_addr_s(const whost_view_t* l, const whost_view_t* const restrict r);
	int whost_view_compare_last_seen_online_s(const whost_view_t* l, const whost_view_t* const restrict r);
	int whost_view_compare_op_result_s(const whost_view_t* l, const whost_view_t* const restrict r);

	BOOL whost_view_update_host_s(whost_view_t* hw, const whost_t* const restrict host);

	BOOL whost_view_set_display_name_s(whost_view_t* hw, const char* const restrict display_name);
	BOOL whost_view_set_hostname_s(whost_view_t* hw, const char* const restrict hostname);
	BOOL whost_view_set_sockarrd_s(whost_view_t* hw, const struct sockaddr* const restrict sa);

	/**
	 * Sets host(view)'s IP address.
	 *
	 * @param hw Host(view) to update.
	 * @param addr IP address in network byte order.
	 *
	 * @return TRUE if the field was updated, otherwise FALSE.
	 */
	BOOL whost_view_set_sockaddr_int_s(whost_view_t* const restrict hw, uint32_t addr);

	BOOL whost_view_set_ip_addr_s(whost_view_t* hw, const char* const restrict ip_addr_str);
	BOOL whost_view_set_mac_addr_s(whost_view_t* hw, const byte* const restrict mac_addr);
	BOOL whost_view_set_mac_addr_string_s(whost_view_t* hw, const char* const restrict mac_addr_str);
	BOOL whost_view_set_last_seen_online_s(whost_view_t* const restrict hw, time_t last_seen_online);
	BOOL whost_view_set_op_result_s(whost_view_t* const restrict hw, whost_op_result_t opr);
	void whost_view_set_extra_data_s(whost_view_t* hw, const void* const restrict data, size_t size);
	void whost_view_set_field_change_processed_s(const whost_view_t* const restrict hw, whost_changed_field_t cf);
	whost_changed_field_t whost_field_2_changed_field_s(whost_field_t hf);

#if __cplusplus
}   // Extern C
#endif
