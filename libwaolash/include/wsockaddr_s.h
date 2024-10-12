#pragma once

/*
Include in precompiled header:

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WS2tcpip.h>
#else
#include <netinet/in.h>
#endif
*/

#if __cplusplus
extern "C" {
#endif

	int wsockaddr_addr_is_empty_s(const struct sockaddr* const restrict sa);
	int wsockaddr_compare_s(const struct sockaddr* l, const struct sockaddr* const restrict r);
	int wsockaddr_compare_int_s(const struct sockaddr* const restrict l, uint32_t r);
	struct sockaddr* wsockaddr_from_string_s(const char* const restrict ipAddr);
	const char* wsockaddr_to_string_s(const struct sockaddr* sa,
		char buffer[], int buf_size);
	const char* wsockaddr_int_to_string_s(uint32_t ip_addr,
		char buffer[], int buf_size);
	const char* wsockaddr_in_addr_to_string_s(struct in_addr in_addr,
		char buffer[], int buf_size);
	struct sockaddr* wsockaddr_dup_s(const struct sockaddr* const restrict sa);

	int wsockaddr_compare_sockaddr_in(const struct sockaddr_in* l,
		const struct sockaddr_in* r);
	int wsockaddr_compare_sockaddr_in6(const struct sockaddr_in6* l,
		const struct sockaddr_in6* r);

	/**
	 * Returns integer representation of IPv4 address.
	 *
	 * @param sa struct sockaddr.
	 *
	 * @return integer representation of IPv4
	 * address in network byte order.
	 */
	uint32_t wsockaddr_to_int(const struct sockaddr* const restrict sa);

#if __cplusplus
}   // Extern C
#endif
