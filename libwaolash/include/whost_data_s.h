#pragma once

/*
Include in precompiled header:

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__) && !defined(__NT__)
#include <time.h>
#endif
*/

#include <waolash_t.h>

#if __cplusplus
extern "C" {
#endif

whost_data_t* whost_data_make_s(const char* display_name,
	const char* hostname, const char* ip_addr,
	const char* mac_addr, time_t lso);

void whost_data_free_s(whost_data_t* const restrict hd);

#if __cplusplus
}   // Extern C
#endif
