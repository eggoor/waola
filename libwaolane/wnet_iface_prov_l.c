#include "pch.h"

#include "include/arp_cache_entry.h"

#include "wnet_iface_prov_u.h"

#define xstr(s) str(s)
#define str(s) #s

#define ARP_CACHE "/proc/net/arp"
#define ARPBUFSIZ 128
#define ARPENTLEN (ARPBUFSIZ - 1)
#define IPADDSTRLEN 15
#define IPADDRBUFSIZ (IPADDSTRLEN + 1)
#define MACADDRSTRLEN 17
#define MACADDRBUFSIZ (MACADDRSTRLEN + 1)

/* Format for fscanf() to read the 1st and 4th */
#define ARP_LINE_FORMAT "%"xstr(IPADDSTRLEN)"s %*s %*s %"xstr(MACADDRSTRLEN)"s %*s %*s"

BOOL wnet_iface_prov_is_suitable_iface_family(sa_family_t saFamily) {
	return saFamily == AF_INET || saFamily == AF_INET6;
}

void wnet_iface_prov_store_ifaces(wnet_iface_prov_t* self,
	int socket, const struct ifconf* ifconf)
{
	struct ifreq* ifreq = ifconf->ifc_req;
	const int if_len = sizeof *ifreq;
	
	for (int i = 0; i < ifconf->ifc_len;)	{
		wlog_if_level(wll_debug, "Found interface %s, family %d\n",
			ifreq->ifr_name, ifreq->ifr_addr.sa_family);
		wnet_iface_prov_store_iface(self, socket, ifreq);

		ifreq = (struct ifreq*)((char*)ifreq + if_len);
		i += if_len;
	}
}

int wnet_iface_prov_get_iface_index(int socket, const struct ifreq* ifreq)
{
	int ifindex = -1;
	
	if (ioctl(socket, SIOCGIFINDEX, &ifreq) != 0) {
		ifindex = ifreq->ifr_ifindex;
	}
	else {
		wlog_if_level(wll_warning, "SIOCGIFINDEX failed: %s (%d)\n",
			strerror(errno), errno);
	}
	
	return ifindex;
}

uint32_t ifreq_get_mask(const struct ifreq* ifreq)
{
	return wsockaddr_to_int(&ifreq->ifr_netmask);
}

byte* ifreq_get_mac_addr(int socket, const struct ifreq* ifreq)
{
	byte* mac_addr = NULL;

	if (ioctl(socket, SIOCGIFHWADDR, ifreq) == 0) {
		mac_addr = wmac_addr_dup_s((const byte*)ifreq->ifr_hwaddr.sa_data);
	}
	else {
		wlog_if_level(wll_warning, "SIOCGIFNETMASK failed: %s (%d)\n",
			strerror(errno), errno);
	}

	return mac_addr;
}

BOOL wnet_iface_prov_process_cache(wcont4r_t* arp_entries)
{
	BOOL res = FALSE;

	FILE* arp_cache = fopen(ARP_CACHE, "r");
	if (!arp_cache) {
		wlog_if_level(wll_warning, "fopen(\"" ARP_CACHE "\") failed: %s (%d)\n",
			strerror(errno), errno);
		return res;
	}

	// Ignore header
	char header[ARPBUFSIZ];
	if (!fgets(header, sizeof(header), arp_cache)) {
		wlog_if_level(wll_warning, "fgets() failed: %s (%d)\n",
			strerror(errno), errno);
		goto cleanup;
	}

	char ip_addr_str[IPADDRBUFSIZ], mac_addr_str[MACADDRBUFSIZ];

	while (fscanf(arp_cache, ARP_LINE_FORMAT, ip_addr_str, mac_addr_str) == 2) {
		byte* cached_mac_addr = wmac_addr_from_string_s(mac_addr_str);
		if (!cached_mac_addr) {
			wlog_if_level(wll_warning, "Couln't parse '%s' as MAC address\n",
				mac_addr_str);
			continue;
		}

		struct sockaddr_in* sin = (struct sockaddr_in*)wsockaddr_from_string_s(ip_addr_str);
		if (sin) {
			arp_cache_entry_t* ce = walloc_s(sizeof(struct arp_cache_entry));
			if (!ce) {
				wlog_if_level(wll_warning, "No %zuB of free memory for cache entrie\n",
					sizeof(struct arp_cache_entry));
				wfree_s(sin);
				wfree_s(cached_mac_addr);
				continue;
			}
			ce->ipAddr = sin->sin_addr.s_addr;
			wfree_s(sin);
			memcpy(ce->macAddr, cached_mac_addr, ETHER_ADDR_LEN);
			res |= wcont4r_add_s(arp_entries, ce);
		}
		wfree_s(cached_mac_addr);
	}

cleanup:
	fclose(arp_cache);

	return res;
}
