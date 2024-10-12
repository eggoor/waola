/** @file warp_l.c
 * Linux-specific implementation of Waola ARP client.
 */

#include "pch.h"

#include "include/warper_cb_data.h"
#include "warper_u.h"

int warper_setup_communication_device(warper_t* self, const wnet_iface_t* nif)
{
	int rc = 0;
	
	SOCKET s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));

	if (s < 0) {
		rc = errno;
		wlog_if_level(wll_error, "%s::socket() for %s failed: %s (%d)\n",
			__func__, wnet_iface_get_name(nif), strerror(rc), rc);
		goto end;
	}

	struct sockaddr_ll sll;
	memset(&sll, 0, sizeof(struct sockaddr_ll));
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = wnet_iface_get_index(nif);

	if (bind(s, (struct sockaddr*)&sll, sizeof(struct sockaddr_ll)) < 0) {
		rc = errno;
		wlog_if_level(wll_error, "%s::bind() failed: %s (%d)\n",
			__func__, strerror(rc), rc);
		closesocket(s);
		goto end;
	}
	
	warper_set_file_descriptor(self, s);
	
end:
	if (0 != rc) {
		warper_set_errno(self, rc);
	}

	return rc;
}

int warper_receive(const warper_t* self)
{
	int rc = -1;
	int fd = warper_get_file_descriptor(self);

	byte buffer[ARPPCKTSIZ];
	struct timeval timeout = {
		.tv_sec = ARP_TIMEOUT_S,
		.tv_usec = ARP_TIMEOUT_MS * 1000
	};

	fd_set socks;
	FD_ZERO(&socks);

	for (int i = 0; TRUE; ++i) {
		FD_SET(fd, &socks);

		rc = select(fd + 1, &socks, NULL, NULL, &timeout);

		if (rc < 0) {
			wlog_if_level(wll_warning, "%s::select() failed: %s (%d)\n",
				__func__, strerror(errno), errno);
			goto end;
		}

		if (0 == rc) {
			wlog_if_level(wll_debug, "%s::select() timed out\n", __func__);
			break;
		}

		ssize_t resp_len = recvfrom(fd, buffer, ARPPCKTSIZ, 0, NULL, NULL);

		if (-1 == resp_len) {
			wlog_if_level(EBADF == errno ? wll_debug : wll_verbose,
				"%s::recvfrom() failed: %s (%d)\n", __func__, strerror(errno), errno);
			rc = -1;
			goto end;
		}

		struct ethhdr* rcv_resp = (struct ethhdr*)buffer;
		if (rcv_resp->h_proto != htons(ETHERTYPE_ARP)) {
			wlog_if_level(wll_verbose, "%s::recvfrom got non-ARP responce\n",
				__func__);
			rc = -1;
			continue;
		}

		struct ether_arp* ether_arp = (struct ether_arp*)(buffer + sizeof(struct ethhdr));
		if (ether_arp->ea_hdr.ar_op != htons(ARPOP_REPLY)) {
			wlog_if_level(ether_arp->ea_hdr.ar_op == htons(ARPOP_REQUEST) ? wll_trace : wll_warning,
				"%s::recvfrom got invalid opcode %d\n", __func__, ntohs(ether_arp->ea_hdr.ar_op));
			rc = -1;
			continue;
		}

		process_arp_response(self, ether_arp);
	}

	rc = 0;

end:
	return rc;
}
