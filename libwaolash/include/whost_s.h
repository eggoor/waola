#pragma once

/*
 Include in precompiled header:

 #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
 #include <WS2tcpip.h>
 #else
 #include <netinet/if_ether.h>
 #endif
 */

#include <waolash_t.h>

#include "wmac_addr_s.h"

#if __cplusplus
extern "C" {
#endif

	void whost_free(whost_t* self);

	const char* whost_get_display_name_s(const whost_t* self);
	const char* whost_get_hostname_s(const whost_t* self);
	const struct sockaddr* whost_get_sockaddr_s(const whost_t* self);
	const byte* whost_get_mac_addr_s(const whost_t* self);
	time_t whost_get_last_seen_online_s(const whost_t* self);
	whost_changed_field_t whost_get_state_s(const whost_t* self);
	BOOL whost_is_updated_s(const whost_t* self);
	whost_op_result_t whost_get_op_result_s(const whost_t* self);
	BOOL whost_is_field_updated_s(const whost_t* self, whost_field_t hf);
	BOOL whost_data_is_empty_s(const whost_data_t* const restrict hd);

	int whost_compare_s(const whost_t* stored, const whost_t* const restrict discovered);
	int whost_compare_display_name_s(const whost_t* l, const whost_t* const restrict r);
	int whost_compare_hostname_s(const whost_t* l, const whost_t* const restrict r);
	int whost_compare_sockaddr_s(const whost_t* l, const whost_t* const restrict r);
	int whost_compare_mac_addr_s(const whost_t* l, const whost_t* const restrict r);

	BOOL whost_set_hostname_s(whost_t* self, const char* const restrict hostName);
	BOOL whost_set_sockaddr_s(whost_t* self, const struct sockaddr* const restrict sa);

	/**
	 * Sets host's IP address.
	 *
	 * @param self Host to update.
	 * @param addr IP address in network byte order.
	 *
	 * @return TRUE if the field was updated, otherwise FALSE.
	 */
	BOOL whost_set_sockaddr_int_s(whost_t* self, uint32_t addr);

	BOOL whost_set_ip_addr_s(whost_t* self, const char* const restrict ip_addr_string);
	BOOL whost_set_mac_addr_s(whost_t* self, const byte* const restrict mac_addr);
	BOOL whost_set_last_seen_online_s(whost_t* self, time_t lso_string);
	void whost_set_field_change_processed_s(whost_t* self, whost_changed_field_t cf);

#if __cplusplus
}   // Extern C
#endif
