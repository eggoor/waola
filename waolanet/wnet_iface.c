#include "pch.h"

#include "wnet_iface.h"

void test_wnet_iface(void)
{
	int ifindex = 0;
	const char* ifaceName = NULL;
	struct sockaddr* sa = NULL;
	uint32_t mask = 0;
	const byte* mac_addr = NULL;

	waolane_fact4s_t* fact4s = wmake_waolane_fact4s(wmake_cont4r_s,
		wmake_mutex, wmake_thread_cbi, wmake_thread, wmake_net_iface);

	wthread_counter_t* tc = wmake_thread_counter(wmake_mutex);

	wnet_iface_t* nif = wmake_net_iface(fact4s, ifindex,
		ifaceName, sa, mask, mac_addr, tc);

	assert(!nif);

	wthread_counter_free(tc);
	waolane_fact4s_free(fact4s);
}
