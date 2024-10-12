/** @file warper_w.c
* Windows-specific implementation of Waola ARP client.
*/

#include "pch.h"

#include "waolane_fact4s.h"
#include "wnet_iface.h"

#include "warper_w.h"

struct warper {
	int _dummy;
};

warper_t* warper_make_impl(const waolane_fact4s_t* fact4s,
	wthread_counter_t* tc)
{
	static warper_t w = { 0 };
	return &w;
}

int warper_free(warper_t* self)
{
	return 0;
}

int warper_resolve_ip_2_mac(uint32_t self_ip_addr, uint32_t target_ip, byte* mac_addr)
{
	int rc = 0;

	if (!mac_addr) {
		rc = EINVAL;
		goto end;
	}

	ULONG phyAddrLen = ETHER_ADDR_LEN;

	rc = SendARP(target_ip, self_ip_addr, mac_addr, &phyAddrLen);

	if (NO_ERROR != rc && ERROR_BAD_NET_NAME != rc) {
		wlog_if_level(wll_trace, "SendARP returned %d\n", rc);
	}

end:
	return rc;
}

int warper_start_listening(warper_t* self, warper_cb_data_t* cb_data)
{
	return ENOSYS;
}

void warper_wait_for_listening_finished(warper_t* self, BOOL join_thread)
{
}

int warper_get_errno(const warper_t* self)
{
	return ENOSYS;
}

BOOL warper_is_ready_for_arp(const warper_t* self)
{
	return TRUE;
}

int warper_query_arp(const warper_t* self, uint32_t target_ip)
{
	return ENOSYS;
}