#include "pch.h"

#include "waolane_fact4s.h"

#include "wnet_iface_prov_u_priv.h"

void wnet_iface_prov_collect_ifaces(wnet_iface_prov_t* self)
{
	static const int def_ifc_len = 0x1000;
	int ifc_len = def_ifc_len;
	struct ifconf ifc = { 0 };
	
	if (!self->fact4s->make_cont4r) {
		wlog_if_level(wll_warning, "Network interface provider has no container factories\n");
		goto end;
	}

	self->ifList = (* self->fact4s->make_cont4r)(wct_list);

	if (!self->ifList) {
		wlog_if_level(wll_warning, "Unable to make interfaceList\n");
		goto end;
	}

	self->collectionCancelled = FALSE;

	char ch;
	ifc.ifc_buf = &ch;

	int s = socket(AF_INET, SOCK_DGRAM, 0);

	if (s < 0) {
		wlog_if_level(wll_error, "socket() failed: %s (%d)\n",
			strerror(errno), errno);
		goto end;
	}

	ifc.ifc_len = ifc_len;

	while (1) {
		ifc.ifc_buf = walloc_s(ifc.ifc_len);

		if (!ifc.ifc_buf) {
			wlog_if_level(wll_error, "No %dB of free memory for ifConf.ifc_buf\n",
				ifc.ifc_len);
			goto cleanup;
		}

		if (ioctl(s, SIOCGIFCONF, &ifc) < 0) {
			wlog_if_level(wll_error, "ioctl(SIOCGIFCONF) failed: %s (%d)\n",
				strerror(errno), errno);
			goto cleanup;
		}

		if (ifc.ifc_len < ifc_len) {
			break;
		}

		wfree_s(ifc.ifc_buf);
		ifc_len += def_ifc_len;
		ifc.ifc_len = ifc_len;
	}

	wnet_iface_prov_store_ifaces(self, s, &ifc);

cleanup:
	closesocket(s);

end:
	wfree_s(ifc.ifc_buf);
}

void wnet_iface_prov_store_iface(wnet_iface_prov_t* self,
	int socket, const struct ifreq* ifreq) {
	if (wlog_get_level() >= wll_debug) {
		ifreq_log(ifreq);
	}

	if (is_suitable_iface(socket, ifreq)) {
		do_add_iface_to_collection(self, socket, ifreq);
	}
	else {
		wlog_if_level(wll_debug, "\t\tIgnore interface '%s' due to unsuitable properties\n",
			ifreq->ifr_name);
	}
}

static BOOL is_suitable_mac_address(int socket, const struct ifreq* const master_ifreq) {
	struct ifreq ifreq;
	memset(&ifreq, 0, sizeof(ifreq));
	strncpy(ifreq.ifr_name, master_ifreq->ifr_name, IFNAMSIZ);
	byte* mac = ifreq_get_mac_addr(socket, &ifreq);
	BOOL res = !wmac_addr_is_empty_s(mac);
	wmac_addr_free_s(mac);
	
	return res;
}

static BOOL is_suitable_iface_flags(int socket, const char* const ifname) {
	struct ifreq ifreq;
	memset(&ifreq, 0, sizeof(ifreq));
	strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);

	if (ioctl(socket, SIOCGIFFLAGS, &ifreq) < 0) {
		wlog_if_level(wll_warning, "Interface '%s': ioctl(SIOCGIFFLAGS) failed: %d (%d)",
			ifname, strerror(errno), errno);
		return FALSE;
	}

	return ifreq.ifr_flags & IFF_UP
		&& ifreq.ifr_flags & IFF_RUNNING
		&& !(ifreq.ifr_flags & IFF_LOOPBACK);
}

static BOOL is_suitable_iface(int socket, const struct ifreq* const ifreq)
{
	return wnet_iface_prov_is_suitable_iface_family(ifreq->ifr_addr.sa_family)
		&& is_suitable_iface_flags(socket, ifreq->ifr_name)
		&& is_suitable_mac_address(socket, ifreq);
}

static void ifreq_log(const struct ifreq* const ifreq)
{
	static const char* messageFormat = "\tFound '%s' of address family %d, IP address %s\n";

	switch (ifreq->ifr_addr.sa_family) {
	case AF_INET: {
		struct sockaddr_in* sockaddrIn = (struct sockaddr_in*)&ifreq->ifr_addr;
		char astring[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &sockaddrIn->sin_addr, astring, INET_ADDRSTRLEN);
		wlog(wll_debug, messageFormat, ifreq->ifr_name,
			ifreq->ifr_ifru.ifru_addr.sa_family, astring);
		break;
	}
	case AF_INET6: {
		struct sockaddr_in6* sockaddrIn6 = (struct sockaddr_in6*)&ifreq->ifr_addr;
		char astring[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &sockaddrIn6->sin6_addr, astring, INET6_ADDRSTRLEN);
		wlog(wll_debug, messageFormat, ifreq->ifr_name,
			ifreq->ifr_ifru.ifru_addr.sa_family, astring);
		break;
	}
	default: {
		wlog(wll_debug, "\tThe '%s' interface is neither AF_INET nor AF_INET6 (address family is %d)\n",
			ifreq->ifr_name, ifreq->ifr_ifru.ifru_addr.sa_family);
	}
	}
}

static void do_add_iface_to_collection(wnet_iface_prov_t* self,
	int socket, const struct ifreq* master_ifreq)
{
	struct sockaddr* sa = NULL;

	sa = wsockaddr_dup_s(&(master_ifreq->ifr_addr));

	int ifindex = wnet_iface_prov_get_iface_index(socket, master_ifreq);

	if (wnet_iface_prov_is_already_collected(self, ifindex,
		master_ifreq->ifr_name, sa->sa_family)) {
		wlog_if_level(wll_debug, "Interface #%d '%s' is skipped because already collected\n",
			ifindex, master_ifreq->ifr_name);
		goto cleanup;
	}

	uint32_t ipv4_mask = 0;

	if (AF_INET == master_ifreq->ifr_addr.sa_family) {
		struct ifreq ifreq;
		memset(&ifreq, 0, sizeof master_ifreq);
		strncpy(ifreq.ifr_name, master_ifreq->ifr_name, IFNAMSIZ);

		if (ioctl(socket, SIOCGIFNETMASK, &ifreq) != 0) {
			wlog_if_level(wll_error, "Interface %s: SIOCGIFNETMASK failed: %s (%d)\n",
				master_ifreq->ifr_name, strerror(errno), errno);
			goto cleanup;
		}

		ipv4_mask = wsockaddr_to_int(&ifreq.ifr_addr);
		wlog_if_level(wll_debug, "\t\tInterface '%s': subnet mask 0x%08X\n",
			master_ifreq->ifr_name, ipv4_mask);

		if (UINT32_MAX == ipv4_mask) {
			wlog_if_level(wll_debug, "\t\tSkipping nterface '%s' with subnet mask 0x%08X\n",
				master_ifreq->ifr_name, ipv4_mask);
			goto cleanup;
		}
	}

	byte* mac = ifreq_get_mac_addr(socket, master_ifreq);

	wnet_iface_t* nif = (* self->fact4s->make_net_iface)(self->fact4s, ifindex,
		master_ifreq->ifr_name, sa, ipv4_mask, mac, self->threadCounter);

	wfree_s(mac);

	if (nif) {
		BOOL added = wcont4r_add_s(self->ifList, nif);

		if (wlog_get_level() >= wll_info) {
			const struct sockaddr* sa = wnet_iface_get_sockaddr(nif);
			char ip_addr_string[INET6_ADDRSTRLEN];
			wsockaddr_to_string_s(sa, ip_addr_string, INET6_ADDRSTRLEN);
			wlog(wll_verbose, "Saving network interface %s (%s)\n",
				wnet_iface_get_name(nif), ip_addr_string);
		}

		if (!added) {
			wlog_if_level(wll_warning, "Unable to make network interface list item\n");
				wnet_iface_free(nif);
		}
	}
	else {
		wlog_if_level(wll_warning, "Unable to make network interface item\n");
	}

cleanup:
	wfree_s(sa);
}
