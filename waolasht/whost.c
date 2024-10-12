#include "pch.h"

#include "whost.h"

void test_whost(void)
{
	const char* display_name = NULL;
	const char* hostname = NULL;
	struct sockaddr* sockaddr = NULL;
	const byte* mac_addr = NULL;
	time_t lso = 0;

	whost_t* host = wmake_host(display_name, hostname, sockaddr, mac_addr, lso);

	assert(host);

	assert(whost_get_display_name_s(host) == NULL);
	assert(whost_get_hostname_s(host) == NULL);
	assert(whost_get_sockaddr_s(host) == NULL);
	const byte* mac_addr2 = whost_get_mac_addr_s(host);
	assert(mac_addr2);

	for (int i = ETHER_ADDR_LEN; i --> 0;) {
		assert(0 == mac_addr2[i]);
	}

	assert(whost_get_last_seen_online_s(host) == 0);
	assert(whost_get_state_s(host) == cf_unchanged);
	assert(whost_is_updated_s(host) == FALSE);
	assert(whost_get_op_result_s(host) == wor_undefined);

	for (int i = 0; i < hwf_count; ++i) {
		whost_field_t hf = i;
		assert(whost_is_field_updated_s(host, hf) == FALSE);
	}

	whost_data_t* hd = whost_data_make_s(display_name, hostname,
		NULL, NULL, lso);
	assert(whost_data_is_empty_s(hd) == TRUE);
	whost_data_free_s(hd);

	display_name = "gateway";
	hostname = "router";
	time(&lso);

	hd = whost_data_make_s(display_name, hostname,
		"172.27.49.214", "01:23:45:67:89:AB", lso);
	whost_t* host2 = wmake_host_from_data(hd);
	whost_data_free_s(hd);

	assert(whost_get_state_s(host2) == (cf_all & ~cf_op_result));
	whost_set_field_change_processed_s(host2, cf_all);
	assert(whost_get_state_s(host2) == cf_unchanged);

	assert(whost_compare_s(host, host2) < 0);
	assert(whost_compare_display_name_s(host, host2) < 0);
	assert(whost_compare_hostname_s(host, host2) < 0);
	assert(whost_compare_sockaddr_s(host, host2) < 0);
	assert(whost_compare_mac_addr_s(host, host2) < 0);

	assert(whost_set_hostname_s(host, NULL) == FALSE);
	assert(whost_set_hostname_s(host, "") == FALSE);
	assert(whost_set_hostname_s(host, hostname) == TRUE);

	sockaddr = wsockaddr_from_string_s("172.27.49.214");

	assert(whost_set_sockaddr_s(host, sockaddr) == TRUE);

	int ip_addr = IP_ADDRESS(172, 27, 49, 214);
	assert(whost_set_sockaddr_int_s(host, htonl(ip_addr)) == FALSE);
	assert(whost_set_sockaddr_int_s(host, htonl(++ip_addr)) == TRUE);

	assert(whost_set_ip_addr_s(host, NULL) == FALSE);
	assert(whost_set_ip_addr_s(host, "") == FALSE);
	assert(whost_set_ip_addr_s(host, "Lorem ipsum") == FALSE);
	assert(whost_set_ip_addr_s(host, "172.27.49.214") == TRUE);

	assert(whost_set_mac_addr_s(host, mac_addr) == TRUE);
	assert(whost_set_last_seen_online_s(host, lso) == TRUE);

	assert(whost_compare_s(host, host) == 0);
	assert(whost_compare_s(host, host2) == 0);

	wfree_s(sockaddr);
	whost_free(host);
	whost_free(host2);
}
