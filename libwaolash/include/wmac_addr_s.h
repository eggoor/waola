/** @file wmac_addr_s.h
* Waola MAC address routins.
*/

#pragma once

/*
Include in precompiled header:

#include <stdint.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WS2tcpip.h>
#endif
*/

#include <wdefs.h>

#if __cplusplus
extern "C" {
#endif

	void wmac_addr_free_s(byte* const restrict mac_addr);

	byte* wmac_addr_dup_s(const byte* const restrict src);
	byte* wmac_addr_from_string_s(const char* restrict str);
	BOOL wmac_addr_is_empty_s(const byte* const restrict mac);
	const char* wmac_addr_to_string_s(const byte* const restrict mac_addr,
		char* const restrict buffer);
	int wmac_addr_compare_s(const byte* const restrict l,
		const byte* const restrict r);

#if __cplusplus
}   // Extern C
#endif
