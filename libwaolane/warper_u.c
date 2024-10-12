/** @file warper_u.c
* Unices-specific implementation of Waola ARP client.
*/

#include "pch.h"

#include "waolane_fact4s.h"

#include "warper_priv.h"

#define ARP_REQ_TIMEOUT_MS 128

struct warper
{
#if defined(DEBUG) || defined(_DEBUG)
	const char* id;
#endif
	const waolane_fact4s_t* fact4s;
	wthread_cbi_t* threadCbi;
	wthread_counter_t* threadCounter;
	warper_cb_data_t* arpCbData;
	wmutex_t* mutex;
	wthread_t* thread;
	int fileDescriptor;
	int errNo;
	size_t bufSize;
	byte arpReqBuffer[ARPPCKTSIZ];
};

warper_t* warper_make_impl(const waolane_fact4s_t* fact4s,
	wthread_counter_t* tc)
{
	warper_t* warper = walloc_s(sizeof(struct warper));

	if (!warper) {
		wlog_if_level(wll_warning, "No %zuB of free memory for ARP client\n",
			sizeof(struct warper));
		goto end;
	}

	warper->threadCbi = (*fact4s->make_thread_cbi)(warper, thread_callbak,
		thread_finished_callback, NULL, NULL);

	if (!warper->threadCbi) {
		wfree_s(warper);
		warper = NULL;
		wlog_if_level(wll_warning, "Unable to make ARP client thread callback info\n");
		goto end;
	}

	warper->mutex = (*fact4s->make_mutex)();
	if (!warper->mutex) {
		wthread_cbi_free(warper->threadCbi);
		wfree_s(warper);
		warper = NULL;
		wlog_if_level(wll_warning, "Unable to make ARP client mutex\n");
		goto end;
	}

	warper->fact4s = fact4s;
	warper->threadCounter = tc;
	warper->arpCbData = NULL;
	warper->fileDescriptor = -1;
	warper->errNo = 0;
	warper->bufSize = 0;
	warper->thread = NULL;
	memset(warper->arpReqBuffer, 0, ARPPCKTSIZ);

#if defined(DEBUG) || defined(_DEBUG)
	warper->id = "ARP client";
#endif

end:
	return warper;
}

int warper_free(warper_t* self)
{
	int rc = 0;

	warper_wait_for_listening_finished(self, TRUE);

	wmutex_free(self->mutex);
	self->mutex = NULL;

	if (self->fileDescriptor > 0) {
		int inner_rc = close(self->fileDescriptor);
		if (0 != inner_rc) {
			wlog_if_level(wll_warning, "%s:close failed: %s (%d)\n",
				__func__, strerror(errno), errno);
			if (0 != rc) {
				rc = inner_rc;
			}
		}
	}

	wthread_cbi_free(self->threadCbi);
	wfree_s(self);

	return rc;
}

void warper_wait_for_listening_finished(warper_t* self,
	BOOL join_thread)
{
	wmutex_lock(self->mutex, INFINITE);

	if (self->thread) {
		wthread_free(self->thread, join_thread ? INFINITE : 0);
		self->thread = NULL;
	}

	if (self->arpCbData) {
		wcont4r_free_s(self->arpCbData->hostDiscovererList);
		wfree_s(self->arpCbData);
		self->arpCbData = NULL;
	}

	wmutex_release(self->mutex);
}

const byte* warper_get_req_buffer(const warper_t* self)
{
	return self->arpReqBuffer;
}

size_t warper_get_buf_size(const warper_t* self)
{
	return self->bufSize;
}

void warper_set_buf_size(warper_t* self, size_t buf_size)
{
	self->bufSize = buf_size;
}

int warper_get_file_descriptor(const warper_t* self)
{
	return self->fileDescriptor;
}

void warper_set_file_descriptor(warper_t* self, int fd)
{
	self->fileDescriptor = fd;
}

int warper_get_errno(const warper_t* self)
{
	return self->errNo;
}

void warper_set_errno(warper_t* self, int err_no)
{
	self->errNo = err_no;
}

BOOL warper_is_ready_for_arp(const warper_t* self)
{
	return self->fileDescriptor > 0;
}

warper_cb_data_t* warper_get_cb_data(const warper_t* self)
{
	return self->arpCbData;
}

int warper_start_listening(warper_t* self, warper_cb_data_t* const cb_data)
{
	int rc = 0;

	if (!wmutex_try_lock(self->mutex)) {
		rc = EBUSY;
		goto end;
	}

	if (self->thread) {
		wlog_if_level(wll_verbose, "ARP client already listening\n");
		wmutex_release(self->mutex);
		goto end;
	}

	const size_t cb_data_size = sizeof(struct warper_cb_data);
	self->arpCbData = walloc_s(cb_data_size);

	if (!self->arpCbData) {
		wlog_if_level(wll_warning, "No %zuB of free memory for ARP cient callback data\n",
			cb_data_size);
		rc = ENOMEM;
		goto end;
	}

	memcpy(self->arpCbData, cb_data, cb_data_size);

	for (int i = 0; !self->thread && i < WDEF_TRY_COUNT; ++i) {
		BOOL increased = wthread_counter_increase_thread_count(self->threadCounter);
		if (increased) {
			self->thread = (* self->fact4s->make_thread)(self->threadCbi);

			if (self->thread) {
				rc = 0;
			}
			else {
				wthread_counter_decrease_thread_count(self->threadCounter);
				rc = ENOMEM;
				wlog_if_level(wll_warning, "Unable to make ARP client listener thread\n");
			}
		}
		else {
			rc = EBUSY;
			wlog_if_level(wll_warning, "Skip creating ARP client listener thread due to lack of threads\n");
		}

		if (!self->thread) {
			wlog_if_level(wll_warning, "Couldn't create ARP client listener thread, will wait for %u ms\n",
				WDEF_WAIT_TIME_MS << i);
			wthread_sleep(WDEF_WAIT_TIME_MS << i);
		}
	}

	wmutex_release(self->mutex);

end:
	return rc;
}

void process_arp_response(const warper_t* self, const struct ether_arp* const ether_arp)
{
	if (wlog_get_level() >= wll_trace) {
		char sender_ip_str[INET_ADDRSTRLEN];
		wsockaddr_int_to_string_s(*(uint32_t*)ether_arp->arp_spa,
			sender_ip_str, INET_ADDRSTRLEN);

		char target_ip_str[INET_ADDRSTRLEN];
		wsockaddr_int_to_string_s(*(uint32_t*)ether_arp->arp_tpa,
			target_ip_str, INET_ADDRSTRLEN);

		char sender_mac_str[3 * ETHER_ADDR_LEN];
		wmac_addr_to_string_s(ether_arp->arp_sha, sender_mac_str);

		wlog(wll_trace, "\t\t\t%s received from %s its MAC address == '%s'\n",
			target_ip_str, sender_ip_str, sender_mac_str);
	}

	warper_cb_data_t* cb_data = warper_get_cb_data(self);

	cb_data->ipAddr = *(uint32_t*)ether_arp->arp_spa;
	cb_data->macAddr = ether_arp->arp_sha;
	(*cb_data->cb)(cb_data);
}

static int thread_callbak(const wthread_cbi_t* const restrict cbi)
{
	const warper_t* self = cbi->threadOwner;
	return warper_receive(self);
}

static void thread_finished_callback(const wthread_cbi_t* const restrict cbi)
{
	warper_t* self = cbi->threadOwner;
	wthread_cbi_t* parent_cbi = self->arpCbData->parentThreadCbi;

	BOOL lckd = wmutex_try_lock_once(self->mutex);
	if (lckd) {
		wlog_if_level(wll_debug, "%s: locked mutex\n", __func__);
		if (self->thread) {
			wthread_free(self->thread, 0);
			self->thread = NULL;
		}

		if (self->arpCbData) {
			wcont4r_free_s(self->arpCbData->hostDiscovererList);
			wfree_s(self->arpCbData);
			self->arpCbData = NULL;
		}

		wmutex_release(self->mutex);
	}

	if (parent_cbi) {
		(*parent_cbi->childThreadFinishedCb)(parent_cbi);
	}
}

BOOL warper_query_arp(const warper_t* self,	uint32_t target_ip)
{
	return warper_do_send(self, target_ip);
}

int warper_resolve_ip_2_mac(uint32_t self_ip_addr, uint32_t target_ip, byte* const mac_addr)
{
	return ENOSYS;
}

void warper_prepare_req_buffer(const warper_t* self, const wnet_iface_t* const nif)
{
	const byte* restrict req_buffer = warper_get_req_buffer(self);
	struct ether_header* restrict ether_header = (struct ether_header*)req_buffer;
	struct ether_arp* restrict ether_arp = (struct ether_arp*)(ether_header + 1);
	
	memset(&ether_header->ether_dhost, 0xff, ETHER_ADDR_LEN);
	
	const byte* const restrict self_mac = wnet_iface_get_mac_addr(nif);
	memcpy(&ether_header->ether_shost, self_mac, ETHER_ADDR_LEN);
	ether_header->ether_type = htons(ETHERTYPE_ARP);
	
	ether_arp->arp_hrd = htons(ARPHRD_ETHER);
	ether_arp->arp_pro = htons(ETHERTYPE_IP);
	ether_arp->arp_hln = ETHER_ADDR_LEN;
	ether_arp->arp_pln = 4;
	ether_arp->arp_op = htons(ARPOP_REQUEST);
	memcpy(ether_arp->arp_sha, self_mac, ETHER_ADDR_LEN);
	
	const struct sockaddr_in* const restrict self_sin = (struct sockaddr_in*)wnet_iface_get_sockaddr(nif);
	uint32_t self_ip = self_sin->sin_addr.s_addr;
	memcpy(ether_arp->arp_spa, &self_sin->sin_addr.s_addr,
		sizeof(self_sin->sin_addr.s_addr));
	
	if (wlog_get_level() >= wll_trace) {
		char self_ip_str[INET_ADDRSTRLEN];
		wsockaddr_int_to_string_s(self_ip, self_ip_str, INET_ADDRSTRLEN);
		char mac_str[3 * ETHER_ADDR_LEN];
		wmac_addr_to_string_s(self_mac, mac_str);
		wlog(wll_trace, "Prepared ARP request to send from %s (%s)\n",
			 self_ip_str, mac_str);
	}
}

int warper_do_send(const warper_t* self, uint32_t target_ip)
{
	int rc = 0;

	const int fd = warper_get_file_descriptor(self);
	const byte* restrict req_buffer = warper_get_req_buffer(self);
	struct ether_arp* restrict ether_arp = (struct ether_arp*)((struct ether_header*)req_buffer + 1);
	
	memcpy(ether_arp->arp_tpa, &target_ip, sizeof(target_ip));
	memset(ether_arp->arp_tha, 0, ETHER_ADDR_LEN);
	
	if (wlog_get_level() >= wll_trace) {
		char target_ip_str[INET6_ADDRSTRLEN];
		wsockaddr_int_to_string_s(target_ip, target_ip_str, INET6_ADDRSTRLEN);
		wlog(wll_trace, "\tSending ARP request to %s against device %d\n",
			 target_ip_str, fd);
	}
	
	ssize_t sent_bytes = 0;

	for (int i = 0; i < ARP_REQ_COUNT; ++i) {
		while (sent_bytes < ARPPCKTSIZ) {
			ssize_t addlen = write(fd, req_buffer + sent_bytes, ARPPCKTSIZ - sent_bytes);
			if (addlen < 0) {
				break;
			}
			sent_bytes += addlen;
		}
		
		if (ARPPCKTSIZ == sent_bytes) {
			rc = 0;
			break;
		}
		else {
			rc = errno;

			if (wlog_get_level() >= wll_verbose) {
				char target_ip_str[INET6_ADDRSTRLEN];
				wsockaddr_int_to_string_s(target_ip, target_ip_str, INET6_ADDRSTRLEN);
				wlog(wll_warning, "%s:write() sending ARP request to %s failed: %s (%d)\n",
					 __func__, target_ip_str, strerror(rc), rc);
			}
			
			if (ENOBUFS == rc) {
				wlog_if_level(wll_verbose, "\t, gonna sleep for %d ms\n",
					ARP_REQ_TIMEOUT_MS << i);
				wthread_sleep(ARP_REQ_TIMEOUT_MS << i);
			}
		}
	}
	
	return rc;
}
