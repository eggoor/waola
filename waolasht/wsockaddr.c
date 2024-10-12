#include "pch.h"

#include "wsockaddr.h"

void test_wsockaddr(void)
{
	assert(wsockaddr_get_len_s(AF_INET) == sizeof(struct sockaddr_in));
	assert(wsockaddr_get_len_s(AF_INET6) == sizeof(struct sockaddr_in6));
	assert(wsockaddr_addr_is_empty_s(NULL) == TRUE);
	struct sockaddr_in sin = {
		.sin_family = AF_INET
	};
	assert(wsockaddr_addr_is_empty_s((const struct sockaddr*)&sin) == TRUE);

	struct sockaddr_in6 sin6 = {
		.sin6_family = AF_INET6
	};
	assert(wsockaddr_addr_is_empty_s((const struct sockaddr*)&sin6) == TRUE);

	struct sockaddr* sa = wsockaddr_from_string_s(NULL);
	assert(!sa);
	sa = wsockaddr_from_string_s("");
	assert(!sa);
	sa = wsockaddr_from_string_s("123");
	assert(!sa);
	sa = wsockaddr_from_string_s("192.168.1.100");
	assert(sa);
	assert(wsockaddr_addr_is_empty_s(sa) == FALSE);
	struct sockaddr* sa2 = wsockaddr_from_string_s("192.168.1.100");
	assert(sa != sa2);
	assert(wsockaddr_addr_is_empty_s(sa2) == FALSE);
	assert(wsockaddr_compare_s(sa, sa2) == 0);
	++sa2->sa_data[2];
	assert(wsockaddr_compare_s(sa, sa2) < 0);

	uint32_t ip_addr = IP_ADDRESS(192, 168, 1, 100);
	assert(wsockaddr_compare_int_s(sa, htonl(ip_addr)) == 0);
	assert(wsockaddr_compare_int_s(sa, htonl(ip_addr + 1)) < 0);
	assert(wsockaddr_compare_int_s(sa, htonl(ip_addr - 1)) > 0);

	wfree_s(sa2);
	wfree_s(sa);
}
