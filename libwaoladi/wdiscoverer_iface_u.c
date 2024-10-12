#include "pch.h"

#include "waoladi.h"

#include "wdiscoverer_iface_platform.h"

static void on_arp_response(warper_cb_data_t* const cb_data);

int wdiscoverer_iface_do_discover(const wthread_cbi_t* const thread_cbi)
{
	int rc = 0;

	wdiscoverer_iface_t* dri = thread_cbi->threadOwner;
	const waoladi_fact4s_t* fact4s = wdiscoverer_iface_get_fact4s(dri);

	wdiscoverer_iface_monitor_t* thread_count_monitor = NULL;
	wcont4r_t* host_list = (*fact4s->make_cont4r)(wct_list);
	
	if (!host_list) {
		wlog_if_level(wll_warning, "%s: Unable to make host list\n",
			__func__);
		goto end;
	}

	rc = begin_discovery(dri);

	if (0 == rc) {
		rc = do_discover(dri);
	}

	wcont4r_free_s(host_list);

	wdiscoverer_iface_notify_discovery_scheduling_finished(dri);
	wlog_if_level(wll_verbose, "Adapter '%s': all discoveries are scheduled\n",
		wnet_iface_get_name(wdiscoverer_iface_get_net_iface(dri)));

	wdiscoverer_iface_monitor_free(thread_count_monitor);

end:
	return rc;
}

static int begin_discovery(wdiscoverer_iface_t* self)
{
	int rc = 0;

	wnet_iface_t* nif = wdiscoverer_iface_get_net_iface(self);

	if (wnet_iface_is_ready_for_arp(nif)) {
		//wdiscoverer_net_t* drn = wdiscoverer_iface_get_parent(self);
		const waoladi_fact4s_t* fact4s = wdiscoverer_iface_get_fact4s(self);

		wcont4r_t* dh_list = (*fact4s->make_cont4r)(wct_list);

		if (!dh_list)
		{
			wlog_if_level(wll_warning, "Unable to make host discoverer list\n");
			rc = ENOMEM;
			goto end;
		}

		warper_cb_data_t cb_data = {
			.context = self,
			.parentThreadCbi = wdiscoverer_iface_get_thread_cbi(self),
			.cb = on_arp_response,
			.hostDiscovererList = dh_list};

		rc = wnet_iface_start_listening(nif, &cb_data);
	}
	else {
		rc = wnet_iface_get_errno(nif);
	}

end:
	return rc;
}

static int do_discover(wdiscoverer_iface_t* self)
{
	int rc = 0;

	wdiscoverer_net_t* drn = wdiscoverer_iface_get_parent(self);
	const wnet_iface_t* nif = wdiscoverer_iface_get_net_iface(self);
	waolast_t* status = wdiscoverer_iface_get_status(self);

	struct sockaddr_in* sin_self = (struct sockaddr_in*)wnet_iface_get_sockaddr(nif);
	struct in_addr* const restrict self_in_addr = &sin_self->sin_addr;

	const uint32_t mask = ntohl(wnet_iface_get_subnet_mask(nif));
	const uint32_t subnet_addr = ntohl(self_in_addr->s_addr) & mask;
	const uint32_t bcast_addr = subnet_addr + ~mask;

	if (wlog_get_level() >= wll_info) {
		char ip_str[INET_ADDRSTRLEN];
		wsockaddr_to_string_s((const struct sockaddr*)sin_self, ip_str, INET_ADDRSTRLEN);
		const uint32_t max_hosts = bcast_addr - subnet_addr - 2;

		wlog_if_level(wll_info, "Gonna discover %u host(s) against adapter '%s' %s/%d\n",
			max_hosts, wnet_iface_get_name(nif), ip_str, calc_set_bits(mask));
	}
	
	for (uint32_t addr = subnet_addr + 1; addr < bcast_addr; ++addr) {
		if (wdiscoverer_net_is_task_cancelled(drn)) {
			break;
		}

		if (htonl(addr) == self_in_addr->s_addr) {
			continue;
		}

		const uint32_t addr_nbo = htonl(addr);
		if (wnet_iface_query_arp(nif, addr_nbo) == 0) {
			wstatus_set(status, wm_discoverer, wdi_sending_arp, &addr_nbo);
		}
		else if(wlog_get_level() >= wll_warning) {
			char ip_addr_str[INET_ADDRSTRLEN];
			wsockaddr_int_to_string_s(addr_nbo, ip_addr_str, INET_ADDRSTRLEN);
			wlog(wll_warning, "Sending ARP request to %s failed\n", ip_addr_str);
		}
	}

end:
	wdiscoverer_iface_on_all_arp_requests_sent(self);

	return rc;
}

static wdiscoverer_host_t* continue_discovery(wdiscoverer_iface_t* self,
	uint32_t target_ip_addr, const byte* target_mac_addr)
{
	return wdiscoverer_iface_make_host_discoverer(self, target_ip_addr, target_mac_addr);
}

static void on_arp_response(warper_cb_data_t* const cb_data)
{
	wdiscoverer_iface_t* dri = cb_data->context;
	wdiscoverer_net_t* drn = wdiscoverer_iface_get_parent(dri);
	
	wdiscoverer_iface_on_host_discovered(dri, cb_data->ipAddr);

	wdiscoverer_host_t* drh = NULL;
	
	BOOL increased = wdiscoverer_net_increment_thread_count(drn);
	if (increased) {
		drh = continue_discovery(dri, cb_data->ipAddr,
			cb_data->macAddr);
		
		if (drh) {
			wcont4r_add_s(cb_data->hostDiscovererList, drh);
		}
		else {
			wdiscoverer_net_decrement_thread_count(drn);
			if (wlog_get_level() >= wll_warning) {
				if (wdiscoverer_net_is_task_cancelled(drn)) {
					wlog_if_level(wll_info, "%s: Discovery task cancelled\n", __func__);
				}
				else {
					wlog(wll_warning, "Unable to make host discoverer\n");
				}
			}
		}
	}
	else {
		wlog_if_level(wll_warning, "Skip creation of host discoverer due to lack of threads\n");
	}
}
