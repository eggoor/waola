#include "pch.h"

#include "wnet_iface_prov_u.h"

void wnet_iface_prov_store_ifaces(wnet_iface_prov_t* self,
	int socket,	const struct ifconf* ifconf)
{
	struct ifreq* ifreq = ifconf->ifc_req;
	
	for (int i = 0; i < ifconf->ifc_len;)	{
		wlog_if_level(wll_debug, "Found interface %s, family %d\n",
			ifreq->ifr_name, ifreq->ifr_addr.sa_family);
		wnet_iface_prov_store_iface(self, socket, ifreq);
		
		int ifcreq_len = IFNAMSIZ + ifreq->ifr_addr.sa_len;
		ifreq = (struct ifreq*)((char*)ifreq + ifcreq_len);
		i += ifcreq_len;
	}
}

int wnet_iface_prov_get_iface_index(int socket, const struct ifreq* const ifreq)
{
	return if_nametoindex(ifreq->ifr_name);
}

BOOL wnet_iface_prov_is_suitable_iface_family(sa_family_t saFamily) {
	return saFamily == AF_INET;
}

byte* ifreq_get_mac_addr(int fd, const struct ifreq* const ifreq)
{
	byte* mac_addr = NULL;
	const char* ifname = ifreq->ifr_name;
	struct ifaddrs* iflist;
	
	if (getifaddrs(&iflist) == 0) {
		for (struct ifaddrs* ifaddr = iflist; ifaddr; ifaddr = ifaddr->ifa_next) {
			if ((ifaddr->ifa_addr->sa_family == AF_LINK)
				&& (strcmp(ifaddr->ifa_name, ifname) == 0)
				&& ifaddr->ifa_addr) {
				const struct sockaddr_dl* sdl = (const struct sockaddr_dl*)ifaddr->ifa_addr;
				mac_addr = wmac_addr_dup_s((byte*)LLADDR(sdl));
				
				if (wlog_get_level() >= wll_verbose) {
					char mac_str[3 * ETHER_ADDR_LEN];
					wmac_addr_to_string_s(mac_addr, mac_str);
					wlog(wll_debug, "\t\tInterface '%s' MAC address is %s\n",
						 ifname, mac_str);
				}
				break;
			}
		}
		
		freeifaddrs(iflist);
	}
	
	return mac_addr;
}

BOOL wnet_iface_prov_process_cache(wcont4r_t* const arp_entries)
{
	//TODO: implement
	return FALSE;
}
