#include "pch.h"

#include "waolare.h"

#include "wresolver_host.h"

#include "wresolver_hosts.h"

int wresolver_hosts_on_resolving_finished(wresolver_hosts_t* self)
{
	int rc = 0;

	self->finishing = TRUE;

	wresolver_host_t* reh = wcont4r_get_first_s(self->resolvers);

	if (!self->cancelled) {
		while (reh) {
			if (!self->isAsync) {
				wresolver_host_wait_finished(reh);
			}
			const whost_view_t* hw = wresolver_host_get_host_view(reh);
			wresolver_hosts_query_arp(self, hw);
			reh = wcont4r_get_next_s(self->resolvers);
		}

		wnet_iface_prov_t* iface_prov = waolare_get_iface_prov(self->parent);
		wnet_iface_prov_wait_for_listening_finished(iface_prov);

		reh = wcont4r_get_first_s(self->resolvers);
	}

	while (reh) {
		wresolver_host_free(reh, self->cancelled ? !self->isAsync : FALSE);
		reh = wcont4r_get_next_s(self->resolvers);
	}

	wcont4r_free_s(self->resolvers);
	self->resolvers = NULL;

	waola_finish();

	if (self->isAsync) {
		waolare_on_resolving_finished((waolare_t*)self->parent);
	}

	self->finishing = FALSE;

	return rc;
}

void wresolver_hosts_query_arp(wresolver_hosts_t* self, const whost_view_t* const hw)
{
	const whost_t* host = whost_view_get_host_s(hw);
	const struct sockaddr* sa = whost_get_sockaddr_s(host);

	if (sa && sa->sa_family == AF_INET) {
		const uint32_t ip_addr = ((const struct sockaddr_in*)sa)->sin_addr.s_addr;
		wnet_iface_prov_t* iface_prov = waolare_get_iface_prov(self->parent);
		wnet_iface_t* nif = wnet_iface_prov_find_iface(iface_prov, ip_addr);

		if (nif) {
			warper_cb_data_t cb_data = {
				.context = self,
				.cb = wresolver_hosts_on_arp_response,
				.parentThreadCbi = self->threadCbi
			};

			wlog_if_level(wll_debug, "\t%s: gonna send ARP request to %s\n",
				whost_view_get_host_id_s(hw), whost_view_get_ip_addr_s(hw));

			wstatus_set(self->status, wm_resolver, wre_sending_arp, &ip_addr);

			if (wnet_iface_start_listening(nif, &cb_data) == 0) {
				if (wnet_iface_query_arp(nif, ip_addr) == 0) {
					wlog_if_level(wll_debug, "Successfully sent ARP request to %s\n",
						whost_view_get_ip_addr_s(hw));
				}
				else {
					wlog_if_level(wll_warning, "\t%s: Unable to send ARP request to %s\n",
						whost_view_get_host_id_s(hw), whost_view_get_ip_addr_s(hw));
				}
				wnet_iface_set_requesting_finished(nif);
			}
			else {
				wlog_if_level(wll_warning, "\t%s: ARP client cannot start listening\n",
					whost_view_get_host_id_s(hw));
			}
		}
		else if (wlog_get_level() >= wll_warning) {
			wlog_if_level(wll_warning, "Network interface corresponding to IP address %s not found\n",
				whost_view_get_ip_addr_s(hw));
		}
	}
}

void wresolver_hosts_on_arp_response(warper_cb_data_t* const cb_data)
{
	const wresolver_hosts_t* rehs = cb_data->context;

	whost_view_t* hw = NULL;
	uint32_t ip_addr = 0;
	wresolver_host_t* reh = wcont4r_get_first_s(rehs->resolvers);

	while (reh) {
		hw = wresolver_host_get_host_view(reh);
		const whost_t* host = whost_view_get_host_s(hw);
		const struct sockaddr* sa = whost_get_sockaddr_s(host);
		ip_addr = ((const struct sockaddr_in*)sa)->sin_addr.s_addr;

		if (ip_addr == cb_data->ipAddr) {
			break;
		}

		reh = wcont4r_get_next_s(rehs->resolvers);
	}

	if (hw) {
		const byte* mac_addr = cb_data->macAddr;

		if (wlog_get_level() >= wll_verbose) {
			char sender_ip_str[INET_ADDRSTRLEN];
			wsockaddr_int_to_string_s(cb_data->ipAddr,
				sender_ip_str, INET_ADDRSTRLEN);

			char sender_mac_str[3 * ETHER_ADDR_LEN] = "(null)";
			if (mac_addr) {
				wmac_addr_to_string_s(mac_addr, sender_mac_str);
			}

			wlog(wll_verbose, "\t\tgot ARP response from %s providing MAC address '%s'\n",
				sender_ip_str, sender_mac_str);
		}

		if (!wmac_addr_is_empty_s(mac_addr)) {
			wstatus_set(rehs->status, wm_resolver, wre_got_arp, &ip_addr);
			whost_view_set_mac_addr_s(hw, mac_addr);
		}
		else {
			wstatus_set(rehs->status, wm_resolver, wre_arp_failed, &ip_addr);
		}

		time_t now;
		time(&now);
		whost_view_set_last_seen_online_s(hw, now);

		if (whost_view_is_updated_s(hw)) {
			const waolava_on_host_modified_cbi_t* cbi = waolare_get_host_modified_cbi(rehs->parent);
			(*cbi->cb)(cbi->vault, hw);
		}
	}
	else if (wlog_get_level() >= wll_warning) {
		char ip_addr_str[INET_ADDRSTRLEN];
		wsockaddr_int_to_string_s(cb_data->ipAddr, ip_addr_str, INET_ADDRSTRLEN);
		wlog(wll_warning, "\t\t\t\tHost with IP address %s not found between resolvers\n",
			ip_addr_str);
	}
}
