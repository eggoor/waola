/** @file whost_data.h
* Waola host data functions.
*/

#pragma once

#include "waolash_t.h"

#include "waolash_exports.h"

#if __cplusplus
extern "C" {
#endif

WAOLASHD_API whost_data_t* whost_data_make(const char* display_name,
	const char* hostname, const char* ip_addr,
	const char* mac_addr, time_t lso);

WAOLASHD_API void whost_data_free(whost_data_t* const hd);

#if __cplusplus
}   // Extern C
#endif
