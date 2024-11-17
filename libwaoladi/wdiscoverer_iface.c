#include "pch.h"

#include "waoladi.h"

#include "wdiscoverer_iface_priv.h"

#define HOST_DISCOVERER_TRY_COUNT 5
#define HOST_DISCOVERER_TIMEOUT_MS 512

struct wdiscoverer_iface
{
#if defined(DEBUG) || defined(_DEBUG)
	const char* id;
#endif
	const waoladi_fact4s_t* fact4s;
	wnet_iface_t* netIface;
	wthread_cbi_t* parentCbi;
	wthread_cbi_t* threadCbi;
	wthread_t* thread;
	waolast_t* status;
	wcont4r_t* hostDiscovererList;
	uint32_t currentlySchedulingAddr;
	wmutex_t* discoveryScheduledEventMutex;
	wevent_t* discoveryScheduledEvent;
	wdiscoverer_iface_monitor_t* progressMonitor;
	BOOL allDiscoveriesAreScheduled;
};

wdiscoverer_iface_t* wdiscoverer_iface_make_impl(wthread_cbi_t* parent_cbi,
	wnet_iface_t* nif, const waolava_merge_host_cbi_t* const restrict mergeHostCbi)
{
	wdiscoverer_iface_t* dri = walloc_s(sizeof(struct wdiscoverer_iface));

	if (!dri) {
		wlog_if_level(wll_warning, "No %zuB of free memory for network interface discoverer\n",
			sizeof(struct wdiscoverer_iface));
		goto end;
	}

	wdiscoverer_net_t* parent = parent_cbi->threadOwner;
	dri->fact4s = wdiscoverer_net_get_fact4s(parent);

	dri->threadCbi = (*dri->fact4s->make_thread_cbi)(dri, wdiscoverer_iface_thread_callbak,
		wdiscoverer_iface_thread_finished_callback, wdiscoverer_iface_child_thread_finished_callback,
		mergeHostCbi);
	if (!dri->threadCbi) {
		wfree_s(dri);
		dri = NULL;
		wlog_if_level(wll_warning,
			"Unable to make interface discoverer thread callback info\n");
		goto end;
	}

	dri->hostDiscovererList = (*dri->fact4s->make_cont4r)(wct_list);
	if (!dri->hostDiscovererList) {
		wthread_cbi_free(dri->threadCbi);
		wfree_s(dri);
		dri = NULL;
		wlog_if_level(wll_warning,
			"Unable to make interface discoverer's host discoverer list\n");
		goto end;
	}

	dri->discoveryScheduledEventMutex = (*dri->fact4s->make_mutex)();
	if (!dri->discoveryScheduledEventMutex) {
		wcont4r_free_s(dri->hostDiscovererList);
		wthread_cbi_free(dri->threadCbi);
		wfree_s(dri);
		dri = NULL;
		wlog_if_level(wll_error, "Unable to make interface discoverer event mutex\n");
		goto end;
	}

	dri->discoveryScheduledEvent = (*dri->fact4s->make_event)();
	if (!dri->discoveryScheduledEvent) {
		wmutex_free(dri->discoveryScheduledEventMutex);
		wcont4r_free_s(dri->hostDiscovererList);
		wthread_cbi_free(dri->threadCbi);
		wfree_s(dri);
		dri = NULL;
		wlog_if_level(wll_error, "Unable to make interface discoverer event\n");
		goto end;
	}

	dri->allDiscoveriesAreScheduled = FALSE;
	dri->parentCbi = parent_cbi;
	dri->netIface = nif;
	dri->status = wdiscoverer_net_get_status(parent);
	dri->currentlySchedulingAddr = 0;
	dri->thread = NULL;
	dri->progressMonitor = NULL;

#if defined(DEBUG) || defined(_DEBUG)
	dri->id = wdiscoverer_iface_get_name(dri);
	wstrncpy_s(dri->threadCbi->ownerId, THR_CBI_ID_BUF_SIZE,
		dri->id, WMIN(THR_CBI_ID_MAX_LEN, strlen(dri->id)));
#endif

end:
	return dri;
}

int wdiscoverer_iface_free(wdiscoverer_iface_t* self, BOOL join_thread)
{
	int rc = 0;

	BOOL allDiscoveriesAreScheduled = FALSE;
	wmutex_lock(self->discoveryScheduledEventMutex, INFINITE);
	allDiscoveriesAreScheduled = self->allDiscoveriesAreScheduled;
	wmutex_release(self->discoveryScheduledEventMutex);

	if (!allDiscoveriesAreScheduled) {
		wevent_wait(self->discoveryScheduledEvent, INFINITE);
	}

	wdiscoverer_iface_free_discovery_scheduled_event(self);
	wmutex_free(self->discoveryScheduledEventMutex);

	wnet_iface_wait_for_warper_listening_finished(self->netIface, join_thread);

	if (self->thread) {
		int rc = wthread_free(self->thread, join_thread ? INFINITE : 0);
		if (rc) {
			wlog_if_level(wll_warning, "%s: wthread_free returned %s (%d)\n",
				__func__, wstrerror_s(rc), rc);
		}
		self->thread = NULL;
	}
	
	wdiscoverer_host_t* drh = wcont4r_get_first_s(self->hostDiscovererList);

	while (drh) {
		wdiscoverer_host_free(drh);
		drh = wcont4r_get_next_s(self->hostDiscovererList);
	}

	wcont4r_free_s(self->hostDiscovererList);

	wthread_cbi_free(self->threadCbi);

	wlog_if_level(wll_info, "Adapter '%s': discovery finished\n",
		wnet_iface_get_name(self->netIface));

	wfree_s(self);

	return rc;
}

BOOL wdiscoverer_iface_discover(wdiscoverer_iface_t* self)
{
	for (int i = 0; !self->thread && i < WDEF_TRY_COUNT; ++i) {
		self->thread = (* self->fact4s->make_thread)(self->threadCbi);
		if (!self->thread) {
			wlog_if_level(wll_info, "Couldn't create network interface discoverer thread, will wait for %u ms\n",
				WDEF_WAIT_TIME_MS << i);
			wthread_sleep(WDEF_WAIT_TIME_MS << i);
		}
	}

	if (!self->thread) {
		wlog_if_level(wll_warning, "Unable to make interface discoverer thread\n");
	}

	return !!self->thread;
}

void wdiscoverer_iface_notify_discovery_scheduling_finished(wdiscoverer_iface_t* self)
{
	wmutex_lock(self->discoveryScheduledEventMutex, INFINITE);
	self->allDiscoveriesAreScheduled = TRUE;
	wmutex_release(self->discoveryScheduledEventMutex);

	wevent_set(self->discoveryScheduledEvent);
}

static void wdiscoverer_iface_free_discovery_scheduled_event(wdiscoverer_iface_t* self)
{
	wevent_free(self->discoveryScheduledEvent);
	self->discoveryScheduledEvent = NULL;
}

const char* wdiscoverer_iface_get_name(const wdiscoverer_iface_t* self)
{
	return wnet_iface_get_name(self->netIface);
}

wdiscoverer_net_t* wdiscoverer_iface_get_parent(const wdiscoverer_iface_t* self)
{
	return self->parentCbi->threadOwner;
}

extern inline wcont4r_t* wdiscoverer_iface_get_host_discoverer_list(const wdiscoverer_iface_t* self)
{
	return self->hostDiscovererList;
}

uint32_t wdiscoverer_iface_get_currently_scheduling_addr(const wdiscoverer_iface_t* self)
{
	return self->currentlySchedulingAddr;
}

wnet_iface_t* wdiscoverer_iface_get_net_iface(const wdiscoverer_iface_t* self)
{
	return self->netIface;
}

const waoladi_fact4s_t* wdiscoverer_iface_get_fact4s(const wdiscoverer_iface_t* self)
{
	return self->fact4s;
}

void wdiscoverer_iface_set_curr_sched_addr(wdiscoverer_iface_t* self, uint32_t ip_addr)
{
	self->currentlySchedulingAddr = ip_addr;
}

void wdiscoverer_iface_on_all_arp_requests_sent(wdiscoverer_iface_t* self)
{
	self->currentlySchedulingAddr = 0;
}

waolast_t* wdiscoverer_iface_get_status(const wdiscoverer_iface_t* self)
{
	return self->status;
}

wthread_cbi_t* wdiscoverer_iface_get_thread_cbi(const wdiscoverer_iface_t* self)
{
	return self->threadCbi;
}

wdiscoverer_host_t* wdiscoverer_iface_make_host_discoverer(wdiscoverer_iface_t* self,
	uint32_t target_ip_addr, const byte* const restrict target_mac_addr)
{
	const wdiscoverer_net_t* drn = wdiscoverer_iface_get_parent(self);

	wdiscoverer_host_t* drh = NULL;
	if (wdiscoverer_net_is_task_cancelled(drn)) {
		goto end;
	}

	const waoladi_fact4s_t* fact4s = wdiscoverer_iface_get_fact4s(self);
	wnet_iface_t* nif = wdiscoverer_iface_get_net_iface(self);
	waolast_t* status = wdiscoverer_iface_get_status(self);

	for (int i = 0; !drh && i < HOST_DISCOVERER_TRY_COUNT; ++i) {
		drh = (*fact4s->make_discoverer_host)(fact4s, nif, target_ip_addr,
			target_mac_addr, status, self->threadCbi);

		if (!drh) {
			wlog_if_level(wll_debug, "\t\tCouldn't create host discoverer, will wait for %u ms\n",
				HOST_DISCOVERER_TIMEOUT_MS << i);
			wthread_sleep(HOST_DISCOVERER_TIMEOUT_MS << i);
		}

		if (wdiscoverer_net_is_task_cancelled(drn)) {
			goto end;
		}
	}

	if (!drh) {
		if (wlog_get_level() >= wll_warning) {
			char ip_addr[INET6_ADDRSTRLEN];
			wsockaddr_int_to_string_s(target_ip_addr, ip_addr, INET6_ADDRSTRLEN);
			wlog(wll_warning, "Unable to make host discoverer, discovery of %s skipped\n",
				ip_addr);
		}
		goto end;
	}

	wdiscoverer_iface_set_curr_sched_addr(self, target_ip_addr);

end:
	return drh;
}

static int wdiscoverer_iface_thread_callbak(const wthread_cbi_t* const cbi)
{
	if (wlog_get_level() >= wll_trace)
	{
		start_monitor_progress(cbi->threadOwner);
	}

	return wdiscoverer_iface_do_discover(cbi);
}

static void wdiscoverer_iface_thread_finished_callback(const wthread_cbi_t* const cbi)
{
	wdiscoverer_iface_t* dri = cbi->threadOwner;

	wdiscoverer_iface_monitor_free(dri->progressMonitor);

	if (0 == dri->parentCbi->errNo && 0 != cbi->errNo) {
		dri->parentCbi->errNo = cbi->errNo;
	}

	(*dri->parentCbi->childThreadFinishedCb)(dri->parentCbi);
}

static void wdiscoverer_iface_child_thread_finished_callback(const wthread_cbi_t* const cbi)
{
	const wdiscoverer_iface_t* self = cbi->threadOwner;

	if (0 == self->parentCbi->errNo && 0 != cbi->errNo) {
		self->parentCbi->errNo = cbi->errNo;
	}

	if (self->parentCbi->childThreadFinishedCb) {
		(self->parentCbi->childThreadFinishedCb)(self->parentCbi);
	}
}

uint32_t wdiscoverer_iface_get_thread_count(const wdiscoverer_iface_t* const dri)
{
	return wdiscoverer_net_get_thread_count(wdiscoverer_iface_get_parent(dri));
}

void wdiscoverer_iface_on_host_discovered(const wdiscoverer_iface_t* self, uint32_t ip_addr)
{
	wstatus_set(self->status, wm_discoverer, wdi_got_arp, &ip_addr);
}

static void start_monitor_progress(wdiscoverer_iface_t* self)
{
	wdiscoverer_net_t* drn = wdiscoverer_iface_get_parent(self);
	const waoladi_fact4s_t* fact4s = wdiscoverer_iface_get_fact4s(self);
	BOOL increased = wdiscoverer_net_increment_thread_count(drn);

	if (increased)
	{
		self->progressMonitor = (*fact4s->make_discoverer_iface_monitor)(fact4s, self->threadCbi);

		if (!self->progressMonitor)
		{
			wlog_if_level(wll_warning, "Unable to make interface discoverer progress monitor\n");
			wdiscoverer_net_decrement_thread_count(drn);
		}
	}
	else
	{
		wlog_if_level(wll_warning, "Skipped creation of thread count monitor due to lack of threads\n");
	}
}
