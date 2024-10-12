/** @file whost_view.h
* Waola network host view interface.

Represents simple host model view which converts
host's data from and to their string representation.
*/

#pragma once

#include "waolash_exports.h"
#include "whost.h"

#if __cplusplus
extern "C" {
#endif

	/**
	 * Gets host display (aka "friendly") name.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Display name set by user or
	 * whost_get_host_name if user didn't set display name.
	 */
	WAOLASHD_API const char* whost_view_get_display_name(const whost_view_t* const hw);

	/**
	 * Gets host exact display name.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Display name set by user or NULL
	 * if user didn't set display name.
	 */
	WAOLASHD_API const char* whost_view_get_exact_display_name(const whost_view_t* const hw);

	/**
	 * Gets hostname.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Hostname if one is set, othervise NULL.
	 */
	WAOLASHD_API const char* whost_view_get_hostname(const whost_view_t* const hw);

	/**
	 * Gets IP address.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Host IP address in dot-decimal
	 * notation if one is set, othervise NULL.
	 */
	WAOLASHD_API const char* whost_view_get_ip_addr(const whost_view_t* const hw);

	/**
	 * Gets MAC (aka "ethernet") address.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Host MAC address if one is set, othervise NULL.
	 */
	WAOLASHD_API const char* whost_view_get_mac_addr(const whost_view_t* const hw);

	/**
	 * Gets datetime when the host was lately response to waola.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Date and time when the host been last
	 * seen online with "%Y.%m.%d %H:%M:%S" format;
	 * NULL if host has never been seen online.
	 */
	WAOLASHD_API const char* whost_view_get_last_seen_online(const whost_view_t* const hw);

	/**
	 * Gets wakeup result. The value is only indicates whether
	 * magic packet has been successfully sent through all
	 * operational interfaces.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Either "Success" or "Fail";
	 * NULL if operation didn't performed.
	 */
	WAOLASHD_API const char* whost_view_get_op_result(const whost_view_t* const hw);

	/**
	 * Gets a value which easily identifies a host.
	 * It's either display name, hostname, IP address
	 * or MAC address depending on whatever one is the
	 * first to be non-empty.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Host identifier.
	 */
	WAOLASHD_API const char* whost_view_get_host_id(const whost_view_t* const hw);

	/**
	 * Gets view underlying host.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Host interface pointer.
	 */
	WAOLASHD_API const whost_t* whost_view_get_host(const whost_view_t* const hw);

	/**
	 * Gets user-supplied extra data.
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Extra data or NULL.
	 */
	WAOLASHD_API void* whost_view_get_extra_data(const whost_view_t* const hw);

	/**
	 * Gets binary mask which shows whether
	 * there are changed host fields since creation
	 * or last /ref whost_set_field_change_processed_s().
	 *
	 * @param hw Host view interface pointer.
	 *
	 * @return Host state.
	 */
	WAOLASHD_API whost_changed_field_t whost_view_get_state(const whost_view_t* const hw);

	/**
	 * Gets state of the given host field.
	 *
	 * @param hw Host view interface pointer.
	 * @param hf Host filed identifier.
	 *
	 * @return TRUE if the field was changed since host creation
	 * or last whost_set_field_update_processed_s(), otherise FALSE.
	 */
	WAOLASHD_API BOOL whost_view_is_field_updated(const whost_view_t* hw,
		whost_field_t hf);

	/**
	 * Gets string representation of the host:
	 * MAC address, IP address, hostname and
	 * display name separated by tabulation.
	 *
	 * @param hw Host view interface pointer.
	  *
	 * @return string representation of the host;
	 * the value must be freed by caller.
	 */
	WAOLASHD_API char* whost_view_to_string(const whost_view_t* const hw);

	/**
	 * Compares views' underlying hosts.
	 * See whost_compare() for more details.
	 *
	 * @param l First host view to compare.
	 * @param r Second host view to compare.
	 *
	 * @return 0 if hosts are equal, > 0 if first host is 'greater'
	 * then second one, < 0 if first host is 'less' then second one.
	 */
	WAOLASHD_API int whost_view_compare(const whost_view_t* l,
		const whost_view_t* r);

	/**
	 * Compares hosts' display names.
	 *
	 * @param l First host view to compare.
	 * @param r Second host view to compare.
	 *
	 * @return 0 if display names are equal, > 0 if first one is 'greater'
	 * then second one, < 0 if first one is 'less' then second one.
	 */
	WAOLASHD_API int whost_view_compare_display_name(const whost_view_t* l,
		const whost_view_t* r);

	/**
	 * Compares hosts' hostnames.
	 *
	 * @param l First host view to compare.
	 * @param r Second host view to compare.
	 *
	 * @return 0 if hostnames are equal, > 0 if first one is 'greater'
	 * then second one, < 0 if first one is 'less' then second one.
	 */
	WAOLASHD_API int whost_view_compare_host_name(const whost_view_t* l,
		const whost_view_t* r);

	/**
	 * Compares hosts' IP addresses.
	 * Comparision is done against integer
	 * IP address representations, not against strings.
	 *
	 * @param l First host view to compare.
	 * @param r Second host view to compare.
	 *
	 * @return 0 if IP addresses are equal, > 0 if first one is greater
	 * then second one, < 0 if first one is less then second one.
	 */
	WAOLASHD_API int whost_view_compare_ip_addr(const whost_view_t* l,
		const whost_view_t* r);

	/**
	 * Compares hosts' MAC addresses.
	 * Comparision is done against integer
	 * MAC address representations, not against strings.
	 *
	 * @param l First host view to compare.
	 * @param r Second host view to compare.
	 *
	 * @return 0 if MAC addresses are equal, > 0 if first one is greater
	 * then second one, < 0 if first one is less then second one.
	 */
	WAOLASHD_API int whost_view_compare_mac_addr(const whost_view_t* l,
		const whost_view_t* r);

	/**
	 * Compares hosts' last seen online times.
	 *
	 * @param l First host view to compare.
	 * @param r Second host view to compare.
	 *
	 * @return 0 if times are equal, > 0 if first one is greater
	 * then second one, < 0 if first one is less then second one.
	 */
	WAOLASHD_API int whost_view_compare_last_seen_online(const whost_view_t* l,
		const whost_view_t* r);

	/**
	 * Compares hosts' wakeup results.
	 *
	 * @param l First host view to compare.
	 * @param r Second host view to compare.
	 *
	 * @return 0 if times are equal, > 0 if first one is greater
	 * then second one, < 0 if first one is less then second one.
	 * Undefined < Success < Fail.
	 */
	WAOLASHD_API int whost_view_compare_op_result(const whost_view_t* l,
		const whost_view_t* r);

	/**
	 * Updates a host with the data from another one.
	 *
	 * @param hw Host(view) to update.
	 * @param host Host to update from.
	 *
	 * @return TRUE if any field was updated, otherwise FALSE.
	 */
	WAOLASHD_API BOOL whost_view_update_host(whost_view_t* hw, const whost_t* const host);

	/**
	 * Sets host's display name.
	 *
	 * @param hw Host(view) to update.
	 * @param display_name Display name to set.
	 *
	 * @return TRUE if the field was updated, otherwise FALSE.
	 */
	WAOLASHD_API BOOL whost_view_set_display_name(whost_view_t* hw,
		const char* display_name);

	/**
	 * Sets host's hostname.
	 *
	 * @param hw Host(view) to update.
	 * @param hostname Hostname to set.
	 *
	 * @return TRUE if the field was updated, otherwise FALSE.
	 */
	WAOLASHD_API BOOL whost_view_set_hostname(whost_view_t* hw,
		const char* hostname);

	/**
	 * Sets host's IP address.
	 *
	 * @param hw Host(view) to update.
	 * @param ip_addr IP address in dot-decimal notation.
	 *
	 * @return TRUE if the field was updated, otherwise FALSE.
	 */
	WAOLASHD_API BOOL whost_view_set_ip_addr(whost_view_t* hw,
		const char* ip_addr);

	/**
	 * Sets host's MAC address.
	 *
	 * @param hw Host(view) to update.
	 * @param mac_addr MAC address to set.
	 *
	 * @return TRUE if the field was updated, otherwise FALSE.
	 */
	WAOLASHD_API BOOL whost_view_set_mac_addr(whost_view_t* hw,
		const byte* mac_addr);

	/**
	 * Sets host's MAC address from string.
	 *
	 * @param hw Host(view) to update.
	 * @param mac_addr_str String representation of MAC address to set.
	 *
	 * @return TRUE if the field was updated, otherwise FALSE.
	 */
	WAOLASHD_API BOOL whost_view_set_mac_addr_string(whost_view_t* hw,
		const char* mac_addr_str);

	/**
	 * Sets user-supplied extra data.
	 *
	 * @param hw Host(view) to update.
	 * @param data Pointer to user-supplied data.
	 * @param size Size of @data.
	 */
	WAOLASHD_API void whost_view_set_extra_data(whost_view_t* hw,
		const void* data, size_t size);

	/**
	 * Resets hosts's field state to cf_unchanged.
	 *
	 * @param hw Host(view) to update.
	 * @param cf Host filed(s) to reset.
	 */
	WAOLASHD_API void whost_view_set_field_change_processed(const whost_view_t* hw,
		whost_changed_field_t cf);

	/**
	 * Gets /ref whost_changed_field_t corresponding to /ref whost_field_t.
	 *
	 * @param hf Host field.
	 *
	 * @return /ref whost_changed_field_t corresponding to hf.
	 */
	WAOLASHD_API whost_changed_field_t whost_field_2_changed_field(whost_field_t hf);

#if __cplusplus
}   // Extern C
#endif
