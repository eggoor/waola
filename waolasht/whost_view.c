#include "pch.h"

#include "whost_view.h"

void test_whost_view(void)
{
	const char* display_name = NULL;
	const char* hostname = NULL;
	struct sockaddr* sockaddr = NULL;
	byte* mac_addr = NULL;
	time_t lso = 0;

	whost_t* host = wmake_host(display_name, hostname, sockaddr, mac_addr, lso);
	assert(host);

	whost_view_t* hw = wmake_host_view(host);
	assert(hw);

	whost_view_free(hw);
}
