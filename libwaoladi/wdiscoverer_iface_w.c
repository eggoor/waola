#include "pch.h"

#include "waoladi.h"

#include "wdiscoverer_iface_platform.h"

#define ARP_REQ_TIMEOUT_MS 16

int wdiscoverer_iface_do_discover(const wthread_cbi_t* const restrict thread_cbi)
{
	wdiscoverer_iface_t* dri = thread_cbi->threadOwner;

	int rc = do_discover(dri);

	wdiscoverer_iface_notify_discovery_scheduling_finished(dri);

	wlog_if_level(wll_verbose, "Adapter '%s': all discoveries are sceduled\n",
		wnet_iface_get_name(wdiscoverer_iface_get_net_iface(dri)));

	return rc;
}

static int begin_discovery(wdiscoverer_iface_t* self)
{
	return 0;
}

static int do_discover(wdiscoverer_iface_t* self)
{
	int rc = 0;

	const wthread_cbi_t* thread_cbi = wdiscoverer_iface_get_thread_cbi(self);
	wdiscoverer_net_t* drn = wdiscoverer_iface_get_parent(self);
	waolast_t* status = wdiscoverer_iface_get_status(self);
	wcont4r_t* dh_list = wdiscoverer_iface_get_host_discoverer_list(self);
	const wnet_iface_t* nif = wdiscoverer_iface_get_net_iface(self);
	struct sockaddr_in* const restrict sin_self = (struct sockaddr_in*)wnet_iface_get_sockaddr(nif);
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

		const uint32_t addr_nbo = htonl(addr);
		if (addr_nbo == self_in_addr->s_addr) {
			continue;
		}

		wstatus_set(status, wm_discoverer, wdi_scheduling_arp, &addr_nbo);

		BOOL increased = wdiscoverer_net_increment_thread_count(drn);
		if (increased) {
			wdiscoverer_host_t* drh = continue_discovery(self, addr_nbo, NULL);

			if (drh) {
				wcont4r_add_s(dh_list, drh);
				wthread_sleep(ARP_REQ_TIMEOUT_MS);
			}
			else {
				wdiscoverer_net_decrement_thread_count(drn);
				if (wlog_get_level() >= wll_info) {
					if (wdiscoverer_net_is_task_cancelled(drn)) {
						wlog_if_level(wll_info, "Discovery task cancelled\n");
						rc = ECANCELED;
					}
					else {
						wlog_if_level(wll_debug, "Unable to make host discoverer\n");
						rc = ENOMEM;
					}
				}
			}
		}
		else {
			wlog_if_level(wll_warning, "Skipped creation of host discoverer due to lack of threads\n");
		}
	}

	return rc;
}

static wdiscoverer_host_t* continue_discovery(wdiscoverer_iface_t* self,
	uint32_t target_ip_addr, const byte* const restrict target_mac_addr)
{
	return wdiscoverer_iface_make_host_discoverer(self,
		target_ip_addr, target_mac_addr);
}
