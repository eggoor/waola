#include "pch.h"

#include "wnet_iface_prov.h"

void test_wnet_iface_prov(void)
{
	wnet_iface_prov_t* nip = wmake_net_iface_prov(NULL,
		NULL, NULL, NULL, NULL, NULL);
	assert(nip);

	const wcont4r_t* nifs = wnet_iface_prov_get_collected_ifaces(nip);
	assert(!nifs);
	wnet_iface_prov_free(nip);

	wthread_counter_t* tc = wmake_thread_counter(wmake_mutex);

	nip = wmake_net_iface_prov(wmake_cont4r_s, wmake_mutex,
		wmake_thread_cbi, wmake_thread, wmake_net_iface, tc);

	assert(nip);
	nifs = wnet_iface_prov_get_collected_ifaces(nip);
	assert(nifs);

	wnet_iface_prov_free(nip);
	wthread_counter_free(tc);
}
