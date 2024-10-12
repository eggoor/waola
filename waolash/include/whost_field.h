/** @file whost_field.h
* Waola host fields definition.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef enum whost_field {
	hwf_undefined = -1
	, hwf_display_name = 0
	, hwf_host_name
	, hwf_ip_address
	, hwf_mac_address
	, hwf_last_seen_online
	, hwf_op_result
	, hwf_count
} whost_field_t;

#if __cplusplus
}   // Extern C
#endif
