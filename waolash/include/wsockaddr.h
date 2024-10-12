/** @file wsockaddr.h
* Waola socket address routines.
*/

#pragma once

#include "waolash_exports.h"

#if __cplusplus
extern "C" {
#endif

WAOLASHD_API const char* wsockaddr_int_to_string(uint32_t ip_addr,
	char buffer[], int buf_size);

#if __cplusplus
}   // Extern C
#endif
