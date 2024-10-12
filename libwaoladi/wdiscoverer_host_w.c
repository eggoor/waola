#include "pch.h"

#include "wdiscoverer_iface.h"

#include "wdiscoverer_host.h"

int wdiscoverer_host_begin_discovery(wdiscoverer_host_t* self)
{
	wnet_iface_t* nif = wdiscoverer_host_get_net_iface(self);
	uint32_t target_ip_addr = wdiscoverer_host_get_target_ip_addr(self);

	byte* mac_addr = wdiscoverer_host_get_mac_addr_buffer(self);
	
	int rc = wnet_iface_resolve_ip_2_mac(nif, target_ip_addr, mac_addr);

	if (0 == rc) {
		if (wlog_get_level() >= wll_verbose) {
			char ip_addr_str[INET_ADDRSTRLEN];
			wsockaddr_int_to_string_s(target_ip_addr, ip_addr_str, INET_ADDRSTRLEN);

			char mac_addr_str[3 * ETHER_ADDR_LEN];
			wmac_addr_to_string_s(mac_addr, mac_addr_str);

			wlog(wll_trace, "\tSuccessfully resolved %s into %s\n",
				ip_addr_str, mac_addr_str);
		}
	}
	else if (wlog_get_level() >= (ERROR_BAD_NET_NAME != rc ? wll_warning : wll_trace)) {
		char ip_addr_str[INET_ADDRSTRLEN];
		wsockaddr_int_to_string_s(target_ip_addr, ip_addr_str, INET_ADDRSTRLEN);
		wlog_if_level(wll_warning, "\t%s: ARP request to %s failed\n",
			__func__, ip_addr_str);
	}

	return rc;
}
