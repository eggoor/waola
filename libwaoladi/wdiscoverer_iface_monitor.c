#include "pch.h"

#include "waoladi_fact4s.h"
#include "wthread.h"
#include "wdiscoverer_net.h"

#include "wdiscoverer_iface_monitor.h"

struct wdiscoverer_iface_monitor
{
#if defined(DEBUG) || defined(_DEBUG)
	const char* id;
#endif
	const wdiscoverer_iface_t* dri;
	wthread_cbi_t* parentCbi;
	BOOL cancelled;
	wthread_cbi_t* threadCbi;
	wthread_t* thread;
	wmutex_t* mutex;
};

wdiscoverer_iface_monitor_t* wdiscoverer_iface_monitor_make_impl(const waoladi_fact4s_t* fact4s,
	wthread_cbi_t* parent_cbi)
{
	wdiscoverer_iface_monitor_t* drim = NULL;

	drim = walloc_s(sizeof(struct wdiscoverer_iface_monitor));

	if (!drim) {
		wlog_if_level(wll_warning, "No %zuB of free memory for network interface discoverer monitor\n",
			sizeof(struct wdiscoverer_iface_monitor));
		goto end;
	}
	
	drim->threadCbi = (*fact4s->make_thread_cbi)(drim, thread_callbak,
		thread_finished_callback, NULL, NULL);
	if (!drim->threadCbi) {
		wfree_s(drim);
		drim = NULL;
		wlog_if_level(wll_warning, "Unable to make interface discoverer monitor thread callback info\n");
		goto end;
	}

	drim->mutex = (*fact4s->make_mutex)();
	if (!drim->mutex) {
		wthread_cbi_free(drim->threadCbi);
		wfree_s(drim);
		wlog_if_level(wll_warning, "Unable to make interface discoverer monitor mutex\n");
		goto end;
	}

	drim->dri = parent_cbi->threadOwner;
	drim->parentCbi = parent_cbi;
	drim->cancelled = FALSE;

#if defined(DEBUG) || defined(_DEBUG)
	drim->id = "Interface discoverer monitor";
	wstrncpy_s(drim->threadCbi->ownerId, THR_CBI_ID_BUF_SIZE,
		drim->id, WMIN(THR_CBI_ID_MAX_LEN, strlen(drim->id)));
#endif

	drim->thread = (*fact4s->make_thread)(drim->threadCbi);

	if (!drim->thread) {
		wmutex_free(drim->mutex);
		wthread_cbi_free(drim->threadCbi);
		wfree_s(drim);
		drim = NULL;
		wlog_if_level(wll_warning, "Unable to make interface discoverer montior thread\n");
	}
end:
	return drim;
}

void wdiscoverer_iface_monitor_free(wdiscoverer_iface_monitor_t* const restrict self)
{
	if (self) {
		cancel(self);
		wthread_free(self->thread, INFINITE);
		wthread_cbi_free(self->threadCbi);
		wmutex_free(self->mutex);
		wfree_s(self);
	}
}

static void cancel(wdiscoverer_iface_monitor_t* const restrict drim)
{
	wmutex_lock(drim->mutex, INFINITE);
	drim->cancelled = TRUE;
	wmutex_release(drim->mutex);
}

static int thread_callbak(const wthread_cbi_t* const restrict thread_cbi)
{
	return monitor_iface_discoverer(thread_cbi);
}

static void thread_finished_callback(const wthread_cbi_t* const cbi)
{
	wdiscoverer_iface_monitor_t* drim = cbi->threadOwner;
	(*drim->parentCbi->childThreadFinishedCb)(drim->parentCbi);
}

static int monitor_iface_discoverer(const wthread_cbi_t* const restrict thread_cbi)
{
	wdiscoverer_iface_monitor_t* drim = thread_cbi->threadOwner;

	while (TRUE) {
		wmutex_lock(drim->mutex, INFINITE);
		if (drim->cancelled) {
			wmutex_release(drim->mutex);
			break;
		}
		const uint32_t addr = wdiscoverer_iface_get_currently_scheduling_addr(drim->dri);

		if (!addr) {
			wlog_if_level(wll_trace, "Discoverer monitor: interface '%s' seems finished scheduling --> break monitoring\n",
				wdiscoverer_iface_get_name(drim->dri));
			wmutex_release(drim->mutex);
			break;
		}

		const uint32_t thread_count = wdiscoverer_iface_get_thread_count(drim->dri);
		char ip_addr_str[INET_ADDRSTRLEN];
		wsockaddr_int_to_string_s(addr, ip_addr_str, INET_ADDRSTRLEN);

		wlog_if_level(wll_trace, "Discoverer monitor: interface '%s' is scheduling %s, thread count == %u ###\n",
			wdiscoverer_iface_get_name(drim->dri), ip_addr_str, thread_count);

		wmutex_release(drim->mutex);

		wthread_sleep(1000);
	}

	return 0;
}
