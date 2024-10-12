#include "pch.h"

#include "wdiscoverer_host.h"

int wdiscoverer_host_begin_discovery(wdiscoverer_host_t* self)
{
	wnet_iface_t* nif = wdiscoverer_host_get_net_iface(self);
	uint32_t target_ip_addr = wdiscoverer_host_get_target_ip_addr(self);
	
	int rc = wnet_iface_query_arp(nif, target_ip_addr);

	if (0 == rc) {
		if (wlog_get_level() >= wll_verbose) {
			char ip_addr_str[INET_ADDRSTRLEN];
			wsockaddr_int_to_string_s(target_ip_addr, ip_addr_str, INET_ADDRSTRLEN);
			wlog(wll_trace, "\t%s: successfully sent ARP request to %s\n",
				__func__, ip_addr_str);
		}
	}
	else if (wlog_get_level() >= wll_warning) {
		char ip_addr_str[INET_ADDRSTRLEN];
		wsockaddr_int_to_string_s(target_ip_addr, ip_addr_str, INET_ADDRSTRLEN);
		wlog_if_level(wll_warning, "\t%s: unable to send ARP request to %s\n",
			__func__, ip_addr_str);
	}

	return rc;
}
