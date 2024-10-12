#include "pch.h"

#include "include/waoladi.h"

#include "wdiscoverer_iface.h"
#include "waoladi_fact4s.h"
#include "wdiscoverer_net.h"
#include "wthread_cbi.h"

#include "wdiscoverer_host_priv.h"

struct wdiscoverer_host
{
#if defined(DEBUG) || defined(_DEBUG)
	char id[16];
#endif
	const waoladi_fact4s_t* fact4s;
	wnet_iface_t* netIface;
	wthread_cbi_t* parentCbi;
	wthread_cbi_t* threadCbi;
	wthread_t* thread;
	waolast_t* status;
	whost_t* host;
	uint32_t targetIpAddr; // Network byte order
	char hostname[NI_MAXHOST];
	byte macAddr[ETHER_ADDR_LEN];
};

wdiscoverer_host_t* wdiscoverer_host_make_impl(const waoladi_fact4s_t* fact4s,
	wnet_iface_t* net_iface, uint32_t target_ip_addr, const byte* mac_addr,
	waolast_t* status, wthread_cbi_t* parent_cbi)
{
	wdiscoverer_host_t* drh = walloc_s(sizeof(struct wdiscoverer_host));

	if (!drh) {
		wlog_if_level(wll_warning, "No %zuB of free memory for host discoverer\n",
			sizeof(struct wdiscoverer_host));
		goto end;
	}

	drh->threadCbi = (*fact4s->make_thread_cbi)(drh, thread_callbak,
		thread_finished_callback, NULL, parent_cbi->extraData);

	if (!drh->threadCbi) {
		wfree_s(drh);
		drh = NULL;
		wlog_if_level(wll_debug, "Unable to make host discoverer thread callback info\n");
		goto end;
	}

	drh->parentCbi = parent_cbi;
	drh->fact4s = fact4s;
	drh->netIface = net_iface;
	drh->targetIpAddr = target_ip_addr;
	drh->status = status;
	drh->host = NULL;

	if (mac_addr) {
		memcpy(drh->macAddr, mac_addr, ETHER_ADDR_LEN);
	}
	else {
		memset(drh->macAddr, 0, ETHER_ADDR_LEN);
	}

	memset(drh->hostname, 0, NI_MAXHOST);

#if defined(DEBUG) || defined(_DEBUG)
	wsockaddr_int_to_string_s(target_ip_addr, drh->id, 16);
	wstrncpy_s(drh->threadCbi->ownerId, THR_CBI_ID_BUF_SIZE,
		drh->id, WMIN(THR_CBI_ID_MAX_LEN, strlen(drh->id)));
#endif

	drh->thread = (*fact4s->make_thread)(drh->threadCbi);

	if (!drh->thread) {
		wthread_cbi_free(drh->threadCbi);
		wfree_s(drh);
		drh = NULL;
		wlog_if_level(wll_debug, "\tUnable to make host discoverer thread\n");
	}

end:
	return drh;
}

int wdiscoverer_host_free(wdiscoverer_host_t* self)
{
	int rc = 0;

	if (self->thread) {
		rc = wthread_free(self->thread, INFINITE);
	}
	wthread_cbi_free(self->threadCbi);
	wfree_s(self);

	return rc;
}

wdiscoverer_iface_t* wdiscoverer_host_get_parent(const wdiscoverer_host_t* self)
{
	return self->parentCbi->threadOwner;
}

wnet_iface_t* wdiscoverer_host_get_net_iface(const wdiscoverer_host_t* self)
{
	return self->netIface;
}

uint32_t wdiscoverer_host_get_target_ip_addr(const wdiscoverer_host_t* self)
{
	return self->targetIpAddr;
}

byte* wdiscoverer_host_get_mac_addr_buffer(wdiscoverer_host_t* self)
{
	return self->macAddr;
}

static int thread_callbak(const wthread_cbi_t* const restrict cbi)
{
	int rc = 0;
	
	wdiscoverer_host_t* drh = cbi->threadOwner;
	BOOL mac_resolved = !wmac_addr_is_empty_s(drh->macAddr);

	if (mac_resolved) {
		wdiscoverer_host_make_host(drh);
		wdiscoverer_host_add_to_vault(drh);
		rc = resolve_hostname(drh);
	}
	else {
		rc = discover_host(drh);
	}
	
	return rc;
}

static void thread_finished_callback(const wthread_cbi_t* const restrict cbi)
{
	wdiscoverer_host_t* drh = cbi->threadOwner;
	(*drh->parentCbi->childThreadFinishedCb)(drh->parentCbi);
}

static int discover_host(wdiscoverer_host_t* self)
{
	int rc = 0;

	const wdiscoverer_net_t* drn = get_discoverer_net(self);
	const uint32_t target_ip_addr = self->targetIpAddr;

	if (wdiscoverer_net_is_task_cancelled(drn)) {
		rc = ECANCELED;
		goto end;
	}

	if (wlog_get_level() >= wll_trace) {
		char ip_addr_str[INET_ADDRSTRLEN];
		wsockaddr_int_to_string_s(target_ip_addr,
			ip_addr_str, INET_ADDRSTRLEN);
		wlog(wll_trace, "\t%s: gonna send ARP request to %s\n",
			__func__, ip_addr_str);
	}

	wstatus_set(self->status, wm_discoverer, wdi_sending_arp, &target_ip_addr);

	rc = wdiscoverer_host_begin_discovery(self);

	if (0 == rc) {
		if (!self->host) {
			wdiscoverer_host_make_host(self);
		}
		else {
			whost_set_mac_addr_s(self->host, self->macAddr);
		}

		if (wlog_get_level() >= wll_trace) {
			char ip_addr_str[INET_ADDRSTRLEN];
			wsockaddr_int_to_string_s(target_ip_addr,
				ip_addr_str, INET_ADDRSTRLEN);

			char mac_str[3 * ETHER_ADDR_LEN];
			wmac_addr_to_string_s(self->macAddr, mac_str);

			wlog(wll_trace, "\t%s: gonna add %s %s to the vault\n",
				__func__, mac_str, ip_addr_str);
		}

		wdiscoverer_host_add_to_vault(self);
		rc = resolve_hostname(self);
	}

	if (67L /*Windows ERROR_BAD_NET_NAME*/ == rc) {
		rc = 0;
	}

end:
	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}

static int resolve_hostname(wdiscoverer_host_t* self)
{
	if (wdiscoverer_net_is_task_cancelled(get_discoverer_net(self))) {
		return 0;
	}

	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = self->targetIpAddr
	};
	const struct sockaddr* sa = (struct sockaddr*)&sin;

	if (wlog_get_level() >= wll_debug) {
		char ip_addr_str[INET6_ADDRSTRLEN];
		wlog(wll_debug, "%s: gonna resolve hostname\n",
			wsockaddr_to_string_s(sa, ip_addr_str, INET6_ADDRSTRLEN));
	}

	BOOL resolved = wresolver_sockaddr_name_resolve(sa, self->hostname,
		NI_MAXHOST, self->status);

	if (wlog_get_level() >= wll_verbose) {
		char ip_addr_str[INET6_ADDRSTRLEN];
		wsockaddr_to_string_s(sa, ip_addr_str, INET6_ADDRSTRLEN);

		if (resolved) {
			wlog(wll_verbose, "%s has resolved to '%s'\n",
				ip_addr_str, self->hostname);
		}
		else {
			wlog(wll_verbose, "Unable to resolve %s\n",
				ip_addr_str);
		}
	}

	if (resolved) {
		if (!self->host) {
			wdiscoverer_host_make_host(self);
		}
		else {
			whost_set_hostname_s(self->host, self->hostname);
		}

		if (wlog_get_level() >= wll_trace) {
			char ip_addr_str[INET_ADDRSTRLEN];
			wsockaddr_int_to_string_s(self->targetIpAddr, ip_addr_str, INET_ADDRSTRLEN);

			char mac_str[3 * ETHER_ADDR_LEN];
			wmac_addr_to_string_s(self->macAddr, mac_str);

			wlog(wll_trace, "\t%s: gonna add %s %s %s to the vault\n",
				__func__, mac_str, ip_addr_str, self->hostname);
		}

		wdiscoverer_host_add_to_vault(self);
	}

	return 0;
}

static void wdiscoverer_host_make_host(wdiscoverer_host_t* self)
{
	if (self->host) {
		wlog_if_level(wll_trace, "There is a maiden host already\n");
		return;
	}

	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = self->targetIpAddr
	};

	const struct sockaddr* sa = (struct sockaddr*)&sin;

	time_t now;
	time(&now);

	for (int i = 0; !self->host && i < WDEF_TRY_COUNT; ++i) {
		self->host = (self->fact4s->make_host)(NULL,
			self->hostname, sa, self->macAddr, now);
		if (!self->host) {
			wlog_if_level(wll_info, "Couldn't create host, will wait for %u ms\n",
				WDEF_WAIT_TIME_MS << i);
			wthread_sleep(WDEF_WAIT_TIME_MS << i);
		}
	}

	if (self->host) {
		if (wlog_get_level() >= wll_trace) {
			char ip_str[22];
			wsockaddr_int_to_string_s(self->targetIpAddr, ip_str, INET_ADDRSTRLEN);

			char mac_str[3 * ETHER_ADDR_LEN];
			wmac_addr_to_string_s(self->macAddr, mac_str);

			wlog_if_level(wll_trace, "\t\t%s made %p %s %s\n",
				__func__, (void*)self->host, mac_str, ip_str);
		}

	}
	else{
		wlog_if_level(wll_warning, "Unable to make host\n");
	}
}

static void wdiscoverer_host_add_to_vault(wdiscoverer_host_t* self)
{
	if (self->host) {
		const waolava_merge_host_cbi_t* cbi = self->threadCbi->extraData;
		BOOL acquired = (*cbi->cb)(cbi->vault, self->host);

		if (acquired) {
			wlog_if_level(wll_debug, "\t\t\t\tNew host acquired by the vault\n");
		}
		else {
			wlog_if_level(wll_verbose, "\t\t\t\tNew host isn't acquired by the vault\n");
			whost_free(self->host);
			self->host = NULL;
		}
	}
	else {
		wlog_if_level(wll_warning, "No host to add\n");
	}
}

static const wdiscoverer_net_t* get_discoverer_net(wdiscoverer_host_t* self)
{
	const wdiscoverer_iface_t* dri = self->parentCbi->threadOwner;
	return wdiscoverer_iface_get_parent(dri);
}
