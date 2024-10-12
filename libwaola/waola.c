/** @file waola.c
* Wake on LAN library.
*
* Broadcasts magic packet for each specified MAC
* address through all active network interfaces.
*/

#include "pch.h"

#include "waola.h"

#define MAGIC_PACKET_SIZE 102
#define WOL_PORT 9

static struct sockaddr_in DestinationAddress4;
static struct sockaddr_in6 DestinationAddress6;

int waola_s(wcont4r_factory cont4r_factory, const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory, const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory, const wnet_iface_prov_factory net_iface_prov_factory,
	const wthread_counter_factory thread_counter_factory,
	const char* const restrict mac_addresses[], size_t count)
{
	int rc = -1;
	byte* mac_addr = NULL;

	wthread_counter_t* tc = thread_counter_factory(mutex_factory);
	if (!tc) {
		wlog_if_level(wll_warning, "Unable to make thread counter\n");
		goto end;
	}

	wcont4r_t* mac_addr_list = make_mac_addr_list_from_strings(cont4r_factory, mac_addresses, count);

	if (!mac_addr_list) {
		wthread_counter_free(tc);
		wlog_if_level(wll_warning, "Unable to create MAC address list\n");
		goto end;
	}

	wnet_iface_prov_t* nip = (*net_iface_prov_factory)(cont4r_factory, mutex_factory,
		thread_cbi_factory, thread_factory, net_iface_factory, tc);

	if (!nip) {
		rc = EX_OSERR;
		wlog_if_level(wll_warning, "Unable to make network interface provider\n");
		goto cleanup;
	}

	const wcont4r_t* iface_list = wnet_iface_prov_get_collected_ifaces(nip);

	rc = waolam(mac_addr_list, iface_list);

	wnet_iface_prov_free(nip);

cleanup:
	mac_addr = wcont4r_get_first_s(mac_addr_list);

	while (mac_addr) {
		wmac_addr_free_s(mac_addr);
		mac_addr = wcont4r_get_next_s(mac_addr_list);
	}

	wthread_counter_free(tc);
	wcont4r_free_s(mac_addr_list);

end:
	return rc;
}

int waolah_s(const wcont4r_factory cont4r_factory, const wmutex_factory mutex_factory,
	const wthread_cbi_factory thread_cbi_factory, const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory, const wnet_iface_prov_factory net_iface_prov_factory,
	const wthread_counter_factory thread_counter_factory,
	whost_view_t* const restrict hosts[], size_t count)
{
	int rc = 0;

	wthread_counter_t* tc = thread_counter_factory(mutex_factory);
	if (!tc) {
		wlog_if_level(wll_warning, "Unable to make thread counter\n");
		goto end;
	}

	wnet_iface_prov_t* nip = (*net_iface_prov_factory)(cont4r_factory, mutex_factory,
		thread_cbi_factory, thread_factory, net_iface_factory, tc);

	if (!nip) {
		wthread_counter_free(tc);
		rc = EX_OSERR;
		wlog_if_level(wll_warning, "Unable to make network interface provider\n");
		goto end;
	}

	const wcont4r_t* iface_list = wnet_iface_prov_get_collected_ifaces(nip);
	rc = waolah(iface_list, hosts, count);
	wnet_iface_prov_free(nip);

end:
	return rc;
}

static int waolam(const wcont4r_t* const restrict mac_addr_list,
	const wcont4r_t* const restrict iface_list)
{
	int rc = 0;

	if (wcont4r_is_empty_s(mac_addr_list)) {
		wlog_if_level(wll_error, "Empty MAC address list\n");
		rc = EINVAL;
		goto end;
	}

	rc = waola_prepare();

	if (rc) {
		goto end;
	}

	char* payload_buff = make_magic_packet();

	if (!payload_buff)
	{
		rc = ENOMEM;
		goto cleanup;
	}

	init_dest_addr();

	wnet_iface_t* nif = wcont4r_get_first_s(iface_list);

	int inner_rc = 0;
	while (nif) {
		sa_family_t sa_family = wnet_iface_get_addr_family(nif);
		SOCKET s = make_socket(nif, sa_family);

		if (SOCKET_ERROR == s) {
			goto next;
		}

		struct sockaddr* dest_sa = get_dest_sockaddr(sa_family);
		int dest_sa_len = get_sockaddr_len(sa_family);

		if (wlog_get_level() >= wll_verbose) {
			char ip_addr_str[INET6_ADDRSTRLEN];
			wsockaddr_to_string_s(dest_sa, ip_addr_str, INET6_ADDRSTRLEN);
			const char* iface_name = wnet_iface_get_name(nif);
			wlog(wll_info, "Through '%s' (%s):\n", iface_name, ip_addr_str);
		}

		inner_rc = wake_all_macs(s, dest_sa,
			dest_sa_len, mac_addr_list, payload_buff);

		if (inner_rc) {
			if (wlog_get_level() >= wll_warning) {
				char ip_addr_str[INET6_ADDRSTRLEN];
				wsockaddr_to_string_s(dest_sa, ip_addr_str, INET6_ADDRSTRLEN);
				const char* iface_name = wnet_iface_get_name(nif);
				wlog(wll_verbose, "\tthrough '%s' (%s):\n", iface_name, ip_addr_str);

				if (0 == rc && EHOSTUNREACH != inner_rc) {
					rc = inner_rc;
				}
			}
		}

		inner_rc = closesocket(s);

	next:

		if (SOCKET_ERROR == inner_rc) {
			inner_rc = get_socket_error_s();
			wlog_if_level(wll_warning, "closesocket failed: %s (%d)\n",
				wstrerror_s(inner_rc), inner_rc);
		}

		if (inner_rc && !rc) {
			rc = inner_rc;
		}

		nif = wcont4r_get_next_s(iface_list);
	}

	wfree_s(payload_buff);

cleanup:
	inner_rc = waola_finish();

	if (inner_rc && !rc) {
		rc = inner_rc;
	}

end:
	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}

static int waolah(const wcont4r_t* iface_list,
	whost_view_t* const restrict hosts[],
	size_t count)
{
	int rc = 0;

	if (0 == count) {
		wlog_if_level(wll_error, "Empty host list\n");
		rc = EX_DATAERR;
		goto end;
	}

	rc = waola_prepare();

	if (rc) {
		goto end;
	}

	char* payloadBuffer = make_magic_packet();

	if (!payloadBuffer)
	{
		rc = EX_OSERR;
		goto cleanup;
	}

	init_dest_addr();

	wnet_iface_t* nif = wcont4r_get_first_s(iface_list);

	for (size_t i = count; i --> 0;) {
		whost_view_set_op_result_s(hosts[i], wor_undefined);
	}

	int inner_rc = 0;
	while (nif) {
		sa_family_t sa_family = wnet_iface_get_addr_family(nif);
		SOCKET s = make_socket(nif, sa_family);

		if (SOCKET_ERROR == s) {
			goto next;
		}

		struct sockaddr* dest_sa = get_dest_sockaddr(sa_family);
		int dest_sa_len = get_sockaddr_len(sa_family);

		inner_rc = wake_all_hosts(s, dest_sa, dest_sa_len,
			hosts, count, payloadBuffer);

		if (inner_rc != 0) {
			const char* ifaceName = wnet_iface_get_name(nif);
			const char* sa_family = wnet_iface_get_addr_family(nif) == AF_INET6
				? "IPv6"
				: "IPv4";
			wlog(wll_warning, "Sending magic packet(s) for the following MAC address(es):\n");
			for (size_t i = count; i --> 0;) {
				wlog(wll_warning, "\t%s\n", whost_view_get_mac_addr_s(hosts[i]));
			}
			wlog(wll_warning, "through '%s' (%s) failed\n", ifaceName, sa_family);
		}
		else if (wlog_get_level() >= wll_info) {
			const char* ifaceName = wnet_iface_get_name(nif);
			const char* sa_family = wnet_iface_get_addr_family(nif) == AF_INET6
				? "IPv6"
				: "IPv4";
			wlog(wll_info, "Successfully sent magic packet(s) for the following MAC address(es):\n");
			for (size_t i = count; i --> 0;) {
				wlog(wll_info, "\t%s\n", whost_view_get_mac_addr_s(hosts[i]));
			}
			wlog(wll_info, "through '%s' (%s)\n", ifaceName, sa_family);
		}

		if (inner_rc && !rc) {
			rc = inner_rc;
		}

	next:
		inner_rc = closesocket(s);

		if (SOCKET_ERROR == inner_rc) {
			inner_rc = get_socket_error_s();
			wlog_if_level(wll_warning, "closesocket failed: %s (%d)\n",
				wstrerror_s(inner_rc), inner_rc);

			if (inner_rc && !rc) {
				rc = inner_rc;
			}
		}

		nif = wcont4r_get_next_s(iface_list);
	}

	wfree_s(payloadBuffer);

	for (size_t i = count; i --> 0;) {
		if (whost_view_get_op_result_s(hosts[i]) == wor_undefined) {
			whost_view_set_op_result_s(hosts[i], wor_success);
		}
	}

cleanup:
	inner_rc = waola_finish();

	if (inner_rc && !rc) {
		rc = inner_rc;
	}

end:
	return rc;
}

static int wake_all_macs(SOCKET s, const struct sockaddr* const restrict dest_sa,
	int dest_sa_len, const wcont4r_t* const restrict mac_addr_list,
	char* const restrict payload)
{
	int rc = 0;

	const byte* mac_addr = wcont4r_get_first_s(mac_addr_list);
	while (mac_addr) {
		for (int i = ETHER_ADDR_LEN; i < MAGIC_PACKET_SIZE; ++i) {
			payload[i] = mac_addr[i % ETHER_ADDR_LEN];
		}

		if (wlog_get_level() >= wll_verbose) {
			char mac_addr_str[3 * ETHER_ADDR_LEN];
			wmac_addr_to_string_s(mac_addr, mac_addr_str);
			wlog(wll_info, "\t sending magic packet for %s\n", mac_addr_str);
		}

		int sentBytes = (int)sendto(s, payload, MAGIC_PACKET_SIZE,
			0, dest_sa, dest_sa_len);

		if (SOCKET_ERROR == sentBytes) {
			int inner_rc = get_socket_error_s();
			char mac_addr_str[3 * ETHER_ADDR_LEN];
			wmac_addr_to_string_s(mac_addr, mac_addr_str);
			wlog_if_level(wll_warning, "Send to %s failed: %s (%d)\n",
				mac_addr_str, wstrerror_s(inner_rc), inner_rc);

			if (0 == rc) {
				rc = inner_rc;
			}
		}
		else if (MAGIC_PACKET_SIZE > sentBytes && wlog_get_level() >= wll_error) {
			char mac_addr_str[3 * ETHER_ADDR_LEN];
			wmac_addr_to_string_s(mac_addr, mac_addr_str);
			wlog(wll_error, "Sent only %d bytes to %s while expected %d\n",
				sentBytes, mac_addr_str, MAGIC_PACKET_SIZE);
		}

		mac_addr = wcont4r_get_next_s(mac_addr_list);
	}

	return rc;
}

static int wake_all_hosts(SOCKET s, struct sockaddr* dest_sa, int dest_sa_len,
	whost_view_t* const restrict hosts[], size_t count, char* payload)
{
	int rc = 0;

	for (size_t i = count; i --> 0;) {
		const whost_t* host = whost_view_get_host_s(hosts[i]);
		const byte* mac_addr = whost_get_mac_addr_s(host);

		if (wmac_addr_is_empty_s(mac_addr)) {
			whost_view_set_op_result_s(hosts[i], wor_fail);
			continue;
		}

		for (int i = ETHER_ADDR_LEN; i < MAGIC_PACKET_SIZE; ++i) {
			payload[i] = mac_addr[i % ETHER_ADDR_LEN];
		}

		int sentBytes = (int)sendto(s, payload, MAGIC_PACKET_SIZE,
			0, dest_sa, dest_sa_len);

		if (SOCKET_ERROR == sentBytes) {
			int inner_rc = get_socket_error_s();
			wlog_if_level(wll_warning, "sendto failed: %s (%d)\n",
				wstrerror_s(inner_rc), inner_rc);

			if (!rc) {
				rc = inner_rc;
			}

			whost_view_set_op_result_s(hosts[i], wor_fail);
		}
		else if (MAGIC_PACKET_SIZE > sentBytes) {
			whost_view_set_op_result_s(hosts[i], wor_fail);
			wlog_if_level(wll_error, "Sent only %d bytes while expected %d\n",
				sentBytes, MAGIC_PACKET_SIZE);
		}
	}

	return rc;
}

static wcont4r_t* make_mac_addr_list_from_strings(const wcont4r_factory cont4r_factory,
	const char* const restrict mac_addr_strings[], size_t size)
{
	wcont4r_t* mac_addr_list = (*cont4r_factory)(wct_list);

	if (!mac_addr_list) {
		wlog_if_level(wll_warning, "Unable to make MAC address list\n");
		goto end;
	}

	for (size_t i = size; i --> 0;)
	{
		byte* macAddress = wmac_addr_from_string_s(mac_addr_strings[i]);
		if (macAddress) {
			BOOL added = wcont4r_add_s(mac_addr_list, macAddress);
			if (!added) {
				wlog_if_level(wll_warning, "Unable to make MAC address list item '%s'\n",
					mac_addr_strings[i]);
				wmac_addr_free_s(macAddress);
			}
		}
		else {
			wlog_if_level(wll_warning, "'%s' is not recognized as a MAC address\n",
				mac_addr_strings[i]);
		}
	}

end:
	return mac_addr_list;
}

static int get_sockaddr_len(sa_family_t sa_family)
{
	size_t socketAddresslength = 0;

	switch (sa_family) {
	case AF_INET: {
		socketAddresslength = sizeof(struct sockaddr_in);
		break;
	}
	case AF_INET6: {
		socketAddresslength = sizeof(struct sockaddr_in6);
		break;
	}
	default: {
		wlog_if_level(wll_warning, "The %d addr family is not supported\n", sa_family);
	}
	}

	return (int)socketAddresslength;
}

static struct sockaddr* get_dest_sockaddr(sa_family_t sa_family)
{
	struct sockaddr* socAddr = NULL;

	switch (sa_family) {
	case AF_INET: {
		socAddr = (struct sockaddr*)&DestinationAddress4;
		break;
	}
	case AF_INET6: {
		socAddr = (struct sockaddr*)&DestinationAddress6;
		break;
	}
	default: {
		wlog_if_level(wll_warning, "The %d addr family is not supported\n", sa_family);
	}
	}

	return socAddr;
}

static SOCKET make_socket(wnet_iface_t* const restrict nif, sa_family_t sa_family)
{
	struct sockaddr* bind_addr = wnet_iface_get_sockaddr(nif);
	size_t namelen = wnet_iface_get_sockaddr_len(nif);

	SOCKET s = socket(sa_family, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == s) {
		int inner_rc = get_socket_error_s();
		wlog_if_level(wll_error, "socket function failed: %s (%d)\n",
			wstrerror_s(inner_rc), inner_rc);
		goto end;
	}

	if (bind_addr) {
		int rc = bind(s, bind_addr, (int)namelen);

		if (SOCKET_ERROR == rc) {
			int inner_rc = get_socket_error_s();
			wlog_if_level(wll_warning, "bind function failed: %s (%d)\n",
				wstrerror_s(inner_rc), inner_rc);
			closesocket(s);
			s = INVALID_SOCKET;
			goto end;
		}
	}

	int True = 1;

	int rc = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&True, sizeof(int));

	if (rc) {
		closesocket(s);
		s = INVALID_SOCKET;
		wlog_if_level(wll_error, "setsockopt function failed: %s (%d)\n",
			wstrerror_s(rc), rc);
	}

end:
	return s;
}

static void init_dest_addr(void)
{
	DestinationAddress4.sin_family = AF_INET;
	DestinationAddress4.sin_port = htons(WOL_PORT);
	DestinationAddress4.sin_addr.s_addr = INADDR_BROADCAST;

	DestinationAddress6.sin6_family = AF_INET6;
	DestinationAddress6.sin6_port = htons(WOL_PORT);
	inet_pton(AF_INET6, "FF02::1", &DestinationAddress6.sin6_addr);
}

static char* make_magic_packet(void)
{
	char* payloadBuffer = walloc_s(MAGIC_PACKET_SIZE);

	if (!payloadBuffer)
	{
		wlog_if_level(wll_warning, "No %dB of free memory for magic packet payload\n",
			MAGIC_PACKET_SIZE);
		goto end;
	}

	for (int i = ETHER_ADDR_LEN; i --> 0;) {
		payloadBuffer[i] = (char)0xFF;
	}

end:
	return payloadBuffer;
}
