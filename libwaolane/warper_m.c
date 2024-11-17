/** @file warper_m.c
* macOS-specific implementation of Waola ARP client.
*/

#include "pch.h"

#include "warper_m.h"

int warper_setup_communication_device(warper_t* self, const wnet_iface_t* const nif)
{
	int rc = ENOMEM;
	
	const char* ifname = wnet_iface_get_name(nif);

	for (int i = 0; 0 != rc && i < WDEF_TRY_COUNT; ++i) {
		rc = do_setup_communication_device(self, ifname);

		if (0 != rc) {
			wlog_if_level(wll_debug, "Couldn't setup communication device, will wait for %u ms\n",
				WDEF_WAIT_TIME_MS << i);
			wthread_sleep(WDEF_WAIT_TIME_MS << i);
		}
	}

	return rc;
}

static int do_setup_communication_device(warper_t* self, const char* const ifname)
{
	static struct bpf_insn bpf_insn[] = {
		// Load word at octet 12
		BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 12),
		// If not ETHERTYPE_ARP, skip next 3 (and return nothing)
		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, ETHERTYPE_ARP, 0, 3),
		// Load word at octet 20
		BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 20),
		// If not ARPOP_REPLY, skip next 1 (and return nothing)
		BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, ARPOP_REPLY, 0, 1),
		// Valid ARP reply received, return message
		BPF_STMT(BPF_RET | BPF_K, ARPPCKTSIZ),
		// Return nothing
		BPF_STMT(BPF_RET | BPF_K, 0),
	};

	static const struct bpf_program filter = {
		sizeof bpf_insn / sizeof(bpf_insn[0]),
		bpf_insn
	};

	int rc = 0;
	
	int fd = -1;
	char device[] = "/dev/bpf000";

	for (int i = 0; i < 1000; ++i)
	{
		snprintf(device, sizeof(device), "/dev/bpf%d", i);

		fd = open(device, O_RDWR);

		if (fd < 0 && EBUSY == errno)
		{
			continue;
		}
		else
		{
			break;
		}
	}

	if (fd < 0)
	{
		rc = errno;	
		wlog_if_level(EACCES != rc ? wll_error : wll_debug,
			"%s: unable to open any bpf device: %s (%d)\n",
			__func__, strerror(rc), rc);
		goto end;
	}

	wlog_if_level(wll_debug, "%s: successfully opened %s\n",
		__func__, device);

	struct ifreq ifreq;
	strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);

	if (ioctl(fd, BIOCSETIF, &ifreq) == -1) {
		rc = errno;	
		wlog_if_level(wll_error, "%s BIOCSETIF failed: %s (%d)\n",
			device, strerror(rc), rc);
		close(fd);
		goto end;
	}

	if (ioctl(fd, BIOCSETF, &filter) == -1) {
		rc = errno;	
		wlog_if_level(wll_error, "%s BIOCSETF failed: %s (%d)\n",
			device, strerror(rc), rc);
		close(fd);
		goto end;
	}

	uint32_t yes = 1;

	if (ioctl(fd, BIOCIMMEDIATE, &yes) == -1) {
		rc = errno;	
		wlog_if_level(wll_error, "%s BIOCIMMEDIATE failed: %s (%d)\n",
			device, strerror(rc), rc);
		close(fd);
		goto end;
	}

	u_int buf_size;
	
	if (ioctl(fd, BIOCGBLEN, &buf_size) == -1) {
		rc = errno;	
		wlog_if_level(wll_error, "%s BIOCGBLEN failed: %s (%d)\n",
			device, strerror(rc), rc);
		close(fd);
		goto end;
	}
	
	warper_set_file_descriptor(self, fd);
	warper_set_buf_size(self, buf_size);
	
	wlog_if_level(wll_debug, "%s: successfully set up %s\n",
		__func__, device);

end:
	if (0 != rc) {
		warper_set_errno(self, rc);
	}

	return rc;
}

int warper_receive(const warper_t* self)
{
	static const struct timeval timeout = {
		.tv_sec = ARP_TIMEOUT_S,
		.tv_usec = ARP_TIMEOUT_MS * 1000
	};

	int rc = 0;
	
	int fd = warper_get_file_descriptor(self);
	size_t buf_size = warper_get_buf_size(self);

	byte* buffer = walloc_s(buf_size);

	if (!buffer) {
		wlog_if_level(wll_warning, "%s: no %zuB of free memory for network response buffer\n",
			__func__, buf_size);
		rc = EX_OSERR;
		goto end;
	}

	if (ioctl(fd, BIOCSRTIMEOUT, &timeout) == -1) {
		wlog_if_level(wll_warning, "%s:BIOCSRTIMEOUT failed: %s (%d)\n",
			__func__, strerror(errno), errno);
		rc = errno;
		goto cleanup;
	}

	ssize_t resp_len = 0;
	
	wlog_if_level(wll_debug, "Start reading communication device\n");

	while ((resp_len = read(fd, buffer, buf_size)) > 0) {
		wlog(wll_trace, "Received ARP response length == %ld\n", resp_len);
		if (resp_len >= sizeof(struct bpf_hdr)) {
			const void* ptr = buffer;
			const struct bpf_hdr* bpf_hdr = (struct bpf_hdr*)ptr;
			const size_t reply_size = bpf_hdr->bh_hdrlen + ARPPCKTSIZ;
			while (resp_len >= reply_size) {
				const size_t ether_arp_offset = bpf_hdr->bh_hdrlen + sizeof(struct ether_header);
				struct ether_arp* ether_arp = (struct ether_arp*)(ptr + ether_arp_offset);
				
				process_arp_response(self, ether_arp);
				
				ptr += reply_size;
				bpf_hdr = (struct bpf_hdr*)ptr;
				resp_len -= reply_size;
			}
		}
	}

	wlog_if_level(wll_debug, "Finihed reading communication device\n");

	rc = 0;

cleanup:
	wfree_s(buffer);

end:
	return rc;
}
