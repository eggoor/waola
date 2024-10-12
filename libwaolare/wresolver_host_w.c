#include "pch.h"

#include "wresolver_hosts.h"

#include "wresolver_host.h"

int wresolver_host_finish_resolve(wresolver_host_t* self)
{
	int rc = 0;

	const whost_t* host = whost_view_get_host_s(self->hostView);
	const struct sockaddr* sa = whost_get_sockaddr_s(host);
	uint32_t ip_addr = wsockaddr_to_int(sa);

	if (0 != ip_addr) {
		wnet_iface_t* nif = wresolver_hosts_find_network_iface(self->parentCbi->threadOwner, ip_addr);

		if (nif) {
			byte mac_addr[ETHER_ADDR_LEN];

			rc = wnet_iface_resolve_ip_2_mac(nif, ip_addr, mac_addr);

			if (0 == rc) {
				time_t now;
				time(&now);

				if (wlog_get_level() >= wll_verbose) {
					char ip_addr_str[INET_ADDRSTRLEN];
					wsockaddr_int_to_string_s(ip_addr, ip_addr_str, INET_ADDRSTRLEN);

					char mac_addr_str[3 * ETHER_ADDR_LEN];
					wmac_addr_to_string_s(mac_addr, mac_addr_str);

					wlog(wll_trace, "\tSuccessfully resolved %s into %s\n",
						ip_addr_str, mac_addr_str);
				}

				whost_view_set_mac_addr_s(self->hostView, mac_addr);
				whost_view_set_last_seen_online_s(self->hostView, now);

				if (whost_view_is_updated_s(self->hostView)) {
					const waolava_on_host_modified_cbi_t* cbi = self->threadCbi->extraData;
					(*cbi->cb)(cbi->vault, self->hostView);
				}
			}
			else if (wlog_get_level() >= (ERROR_BAD_NET_NAME != rc ? wll_warning : wll_trace)) {
				char ip_addr_str[INET_ADDRSTRLEN];
				wsockaddr_int_to_string_s(ip_addr, ip_addr_str, INET_ADDRSTRLEN);
				wlog_if_level(wll_warning, "\t%s: ARP request to %s failed\n",
					__func__, ip_addr_str);
			}
		}
		else if (wlog_get_level() >= wll_warning) {
			char ip_str[INET_ADDRSTRLEN];
			wsockaddr_int_to_string_s(ip_addr, ip_str, INET_ADDRSTRLEN);
			wlog(wll_warning, "Network interface corresponding to IP address %s not found\n",
				ip_str);

			rc = EOTHER;
		}
	}
	else {
		rc = EPROTO;
	}

	if (ERROR_BAD_NET_NAME == rc) {
		rc = 0;
	}

	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}