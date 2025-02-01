#include "pch.h"

#include "include/wresolver_hostname.h"

#include "waolare_fact4s.h"
#include "wresolver_hosts.h"
#include "wresolver_sockaddr_resolve.h"

#include "wresolver_host_priv.h"

wresolver_host_t* wresolver_host_make_impl(wthread_cbi_t* parent_cbi,
	whost_view_t* hw, const waolava_on_host_modified_cbi_t* host_modified_cbi,
	waolast_t* status)
{
	wresolver_host_t* reh = walloc_s(sizeof(struct wresolver_host));

	if (!reh) {
		wlog_if_level(wll_warning, "No %zuB of free memory for host reh\n",
			sizeof(struct wresolver_host));
		goto end;
	}

	wresolver_hosts_t* parent = parent_cbi->threadOwner;
	const waolare_fact4s_t* fact4s = wresolver_hosts_get_fact4s(parent);

	reh->threadCbi = (*fact4s->make_thread_cbi)(reh, thread_callbak,
		thread_finished_callback, child_thread_finished_callback, host_modified_cbi);

	if (!reh->threadCbi) {
		wfree_s(reh);
		reh = NULL;
		wlog_if_level(wll_warning, "Unable to make host resolver thread callback info\n");
		goto end;
	}

	reh->parentCbi = parent_cbi;
	reh->hostView = hw;
	reh->status = status;
	
	whost_view_set_op_result_s(reh->hostView, wor_undefined);

#if defined(DEBUG) || defined(_DEBUG)
	reh->id = whost_view_get_host_id_s(reh->hostView);
	const size_t id_len = reh->id ? WMIN(THR_CBI_ID_MAX_LEN, strlen(reh->id)) : 0;
	wstrncpy_s(reh->threadCbi->ownerId, THR_CBI_ID_BUF_SIZE, reh->id, id_len);
#endif

	reh->thread = (*fact4s->make_thread)(reh->threadCbi);

	if (!reh->thread) {
		wthread_cbi_free(reh->threadCbi);
		wfree_s(reh);
		reh = NULL;
		wlog_if_level(wll_warning, "Unable to make host resolver thread\n");
	}

end:
	return reh;
}

int wresolver_host_free(wresolver_host_t* self, BOOL join_thread)
{
	int rc = 0;

	if (self->thread) {
		rc = wthread_free(self->thread, join_thread ? INFINITE : 0);
	}
	
	wthread_cbi_free(self->threadCbi);
	wfree_s(self);

	return rc;
}

inline wresolver_hosts_t* wresolver_host_get_parent(const wresolver_host_t* self)
{
	return self->parentCbi->threadOwner;
}

void wresolver_host_wait_finished(wresolver_host_t* self)
{
	if (self->thread) {
		wthread_join(self->thread);
		self->thread = NULL;
	}
}

whost_view_t* wresolver_host_get_host_view(const wresolver_host_t* self)
{
	return self->hostView;
}

static int thread_callbak(const wthread_cbi_t* const cbi)
{
	return resolve_host(cbi);
}

static void thread_finished_callback(const wthread_cbi_t* const restrict cbi)
{
	wresolver_host_t* self = cbi->threadOwner;
	(* self->parentCbi->childThreadFinishedCb)(self->parentCbi);
}

static void child_thread_finished_callback(const wthread_cbi_t* const cbi)
{
	wresolver_host_t* self = cbi->threadOwner;
	(* self->parentCbi->childThreadFinishedCb)(self->parentCbi);
}

int resolve_host(const wthread_cbi_t* const restrict cbi)
{
	wresolver_host_t* self = cbi->threadOwner;
	
	int rc = do_resolve_host(self);
	
	if (whost_view_is_updated_s(self->hostView)) {
		const waolava_on_host_modified_cbi_t* host_modified_cbi = cbi->extraData;
		(*host_modified_cbi->cb)(host_modified_cbi->vault, self->hostView);
	}
	
	return rc;
}

static int do_resolve_host(wresolver_host_t* self)
{
	const wresolver_hosts_t* parent = wresolver_host_get_parent(self);
	const whost_t* host = whost_view_get_host_s(self->hostView);
	const byte* mac_addr = whost_get_mac_addr_s(host);
	uint32_t ip_addr = 0;
	
	wlog_if_level(wll_verbose, "Starting to resolve '%s'\n",
		whost_view_get_host_id_s(self->hostView));

	if (!wmac_addr_is_empty_s(mac_addr)) {
		wlog_if_level(wll_debug, "\t%s: gonna lookup host IP address in ARP cache\n",
			whost_view_get_host_id_s(self->hostView));
		ip_addr = wresolver_host_lookup_in_arp_cache(self);
	}

	const struct sockaddr* sa = whost_get_sockaddr_s(host);

	if (0 != ip_addr && wsockaddr_compare_int_s(sa, ip_addr) != 0) {
		whost_view_set_sockaddr_int_s(self->hostView, ip_addr);
		sa = whost_get_sockaddr_s(host);
	}

	if (!wsockaddr_addr_is_empty_s(sa)) {
		wlog_if_level(wll_verbose, "\t%s: starting to resolve IP address %s to hostname\n",
			whost_view_get_host_id_s(self->hostView), whost_view_get_ip_addr_s(self->hostView));
		resolve_hostname(self);
		if (wresolver_hosts_is_cancelled(parent)) {
			return 0;
		}
	}
	else if (!wstr_is_empty_s(whost_get_hostname_s(host))) {
		wlog_if_level(wll_debug, "\t%s: gonna resolve hostname (%s) to IP address\n",
			whost_view_get_host_id_s(self->hostView), whost_view_get_hostname_s(self->hostView));

		wresolver_sockaddr_resolve(self->hostView, self->status);
	}
	
	return wresolver_host_finish_resolve(self);
}

static uint32_t wresolver_host_lookup_in_arp_cache(const wresolver_host_t* self)
{
	uint32_t ip_addr = 0;
	const whost_t* host = whost_view_get_host_s(self->hostView);
	const byte* mac_addr = whost_get_mac_addr_s(host);
	
	if (!wmac_addr_is_empty_s(mac_addr)) {
		const wresolver_hosts_t* parent = wresolver_host_get_parent(self);
		wnet_iface_prov_t* iface_prov = wresolver_hosts_get_iface_prov(parent);
		ip_addr = wnet_iface_prov_lookup_ip_addr_in_arp_cache(iface_prov, mac_addr);

		if (wlog_get_level() >= wll_verbose) {
			char ip_addr_str[INET_ADDRSTRLEN];
			wsockaddr_int_to_string_s(ip_addr, ip_addr_str, INET_ADDRSTRLEN);
			wlog(wll_verbose, "\t%s: finished lookup ARP cache, got IP address %s\n",
				whost_view_get_host_id_s(self->hostView), ip_addr_str);
		}
	}
	
	return ip_addr;
}

static void resolve_hostname(const wresolver_host_t* self)
{
	char hostname[NI_MAXHOST] = {'\0'};
	const whost_t* host = whost_view_get_host_s(self->hostView);
	const struct sockaddr* sa = whost_get_sockaddr_s(host);
	BOOL resolved = wresolver_sockaddr_name_resolve(sa, hostname,
		NI_MAXHOST, self->status);

	if (resolved) {
		wlog_if_level(wll_verbose, "\t%s: successfully resolved IP address %s into '%s'\n",
			whost_view_get_host_id_s(self->hostView), whost_view_get_ip_addr_s(self->hostView),
			hostname);
		whost_view_set_hostname_s(self->hostView, hostname);
	}
}
