/** @file whost.h
* Waola network host model interface.
*
* Represents network host.
*/

#pragma once

#include "waolash_t.h"
#include "waolash_exports.h"

#if __cplusplus
extern "C" {
#endif

/**
 * Gets host display (aka "friendly") name.
 *
 * @param host Host interface pointer.
 *
 * @return Display name.
 */
WAOLASHD_API const char* whost_get_display_name(const whost_t* const host);

/**
 * Gets hostname.
 *
 * @param host Host interface pointer.
 *
 * @return Hostname if one is discovered, othervise NULL.
 */
WAOLASHD_API const char* whost_get_host_name(const whost_t* const host);

/**
 * Gets socket address.
 *
 * @param host Host interface pointer.
 *
 * @return Host socket network address if one is discovered, othervise NULL.
 */
WAOLASHD_API const struct sockaddr* whost_get_sockaddr(const whost_t* const host);

/**
 * Gets MAC (aka "ethernet") address.
 *
 * @param host Host interface pointer.
 *
 * @return Host MAC address if one is discovered, othervise NULL.
 */
WAOLASHD_API const byte* whost_get_mac_addr(const whost_t* const host);

/**
 * Gets datetime when the host was lately response to waola.
 *
 * @param host Host interface pointer.
 *
 * @return Date and time when the host been last seen online.
 */
WAOLASHD_API time_t whost_get_last_seen_online(const whost_t* const host);

/**
 * Gets binary mask which shows whether
 * there are changed host fields since creation
 * or last /ref whost_set_field_change_processed_s().
 *
 * @param host Host interface pointer.
 *
 * @return Host changed field(s) bit mask.
 */
WAOLASHD_API whost_changed_field_t whost_get_state(const whost_t* const host);

/**
 * Gets wakeup result. The value is only indicates whether
 * magic packet has been successfully sent through all
 * operational interfaces.
 *
 * @param host Host interface pointer.
 *
 * @return Wakeup result.
 */
WAOLASHD_API int whost_get_op_result(const whost_t* const host);

/**
 * Compares two hosts.
 * Comparison is done by the following rules:
 *  - if pointers addresses are equal then hosts are equal,
 *  - if MAC addresses are both not NULL and equal then hosts are equal,
 *  - otherwise if {IP addresses, hostnames, display names}
 * 		  are both not NULL and equal then hosts are equal,
 *  - otherwise the same algorithm but ignoring nullability.
 *
 * @param this_host First host to compare.
 * @param other_host Second host to compare.
 *
 * @return 0 if hosts are equal, > 0 if first host is 'greater'
 * then second one, < 0 if first host is 'less' then second one.
 */
WAOLASHD_API int whost_compare(const whost_t* this_host,
	const whost_t* other_host);

#if __cplusplus
}   // Extern C
#endif
