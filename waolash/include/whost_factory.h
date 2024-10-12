#pragma once

#if __cplusplus
extern "C" {
#endif

whost_t* wmake_host(const char* display_name, const char* hostname,
	const struct sockaddr* sockaddr, const byte* const mac_addr, time_t lso);

whost_t* wmake_host_from_data(const whost_data_t* const hd);

#if __cplusplus
}   // Extern C
#endif
