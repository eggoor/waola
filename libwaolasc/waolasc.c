/** @file libwaolasc.h
* Waola scanner implementation.
*/

#include "pch.h"

#include "waolasc.h"

struct waolasc
{
	wthread_counter_t* threadCounter;
	waolava_t* vault;
	waoladi_t* discoverer;
	waolare_t* resolver;
	wcont4r_t* subscribers;
	waolast_t* status;
	wmutex_t* mutex;
	async_operation_data_t asyncDiscoveryCb;
	wtask_t tasks;
};

waolasc_t* waolasc_make_impl(const wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const wevent_factory event_factory,
	const wthread_counter_factory thread_counter_factory,
	const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory,
	const wnet_iface_prov_factory net_iface_prov_factory,
	const whost_factory host_factory,
	const whost_from_data_factory host_from_data_factory,
	const whost_view_factory host_view_factory,
	const waolava_factory waolava_factory,
	const waolava_on_host_modified_cbi_factory waolava_on_host_modified_cbi_factory,
	const waolava_merge_host_cbi_factory waolava_merge_host_cbi_factory,
	const waoladi_factory waoladi_factory,
	const wdiscoverer_net_factory discoverer_net_factory,
	const wdiscoverer_iface_factory discoverer_iface_factory,
	const wdiscoverer_iface_monitor_factory discoverer_iface_monitor_factory,
	const wdiscoverer_host_factory discoverer_host_factory,
	const waolare_factory waolare_factory,
	const wresolver_hosts_factory resolver_hosts_factory,
	const wresolver_host_factory resolver_host_factory,
	const waolast_factory status_factory,
	const waolasc_subscribe_data_t* state_cbi,
	const waolava_subscribe_data_t* vault_cbi)
{
	waolasc_t* scanner = walloc_s(sizeof(struct waolasc));
	if (!scanner) {
		wlog_if_level(wll_warning, "No %zuB of free memory for scanner\n",
			sizeof(struct waolasc));
		goto end;
	}

	scanner->threadCounter = thread_counter_factory(mutex_factory);
	if (!scanner->threadCounter) {
		wfree_s(scanner);
		scanner = NULL;
		wlog_if_level(wll_warning, "Unable to make thread counter\n");
		goto end;
	}

	scanner->status = (*status_factory)(mutex_factory);
	if (!scanner->status) {
		wthread_counter_free(scanner->threadCounter);
		wfree_s(scanner);
		scanner = NULL;
		wlog_if_level(wll_warning, "Unable to make scanner status\n");
		goto end;
	}

	scanner->vault = (*waolava_factory)(cont4r_factory, mutex_factory, host_from_data_factory,
		host_view_factory, waolava_on_host_modified_cbi_factory, waolava_merge_host_cbi_factory);
	if (!scanner->vault) {
		wstatus_free(scanner->status);
		wthread_counter_free(scanner->threadCounter);
		wfree_s(scanner);
		scanner = NULL;
		wlog_if_level(wll_warning, "Unable to make a vault\n");
		goto end;
	}

	const waolava_merge_host_cbi_t* merge_host_cbi = waolava_get_merge_host_cbi(scanner->vault);

	scanner->discoverer = (*waoladi_factory)(cont4r_factory, mutex_factory, event_factory,
		thread_cbi_factory, thread_factory, net_iface_factory,
		net_iface_prov_factory, host_factory, discoverer_net_factory,
		discoverer_iface_factory, discoverer_iface_monitor_factory,
		discoverer_host_factory, scanner->threadCounter, scanner->status,
		merge_host_cbi);
	if (!scanner->discoverer) {
		waolava_free(scanner->vault);
		wstatus_free(scanner->status);
		wthread_counter_free(scanner->threadCounter);
		wfree_s(scanner);
		scanner = NULL;
		wlog_if_level(wll_warning, "Unable to make network discoverer\n");
		goto end;
	}

	const waolava_on_host_modified_cbi_t* host_modified_cbi = waolava_get_host_modidied_cbi(scanner->vault);

	scanner->resolver = (*waolare_factory)(cont4r_factory, mutex_factory, event_factory,
		thread_cbi_factory, thread_factory, net_iface_factory, net_iface_prov_factory,
		resolver_hosts_factory, resolver_host_factory,  scanner->threadCounter,
		scanner->status, host_modified_cbi);
	if (!scanner->resolver) {
		waoladi_free(scanner->discoverer);
		waolava_free(scanner->vault);
		wstatus_free(scanner->status);
		wthread_counter_free(scanner->threadCounter);
		wfree_s(scanner);
		scanner = NULL;
		wlog_if_level(wll_warning, "Unable to make a resolver\n");
		goto end;
	}

	scanner->mutex = (*mutex_factory)();
	if (!scanner->mutex) {
		waolare_free(scanner->resolver);
		waoladi_free(scanner->discoverer);
		waolava_free(scanner->vault);
		wstatus_free(scanner->status);
		wthread_counter_free(scanner->threadCounter);
		wfree_s(scanner);
		scanner = NULL;
		wlog_if_level(wll_warning, "Unable to make scanner mutex\n");
		goto end;
	}

	scanner->subscribers = (*cont4r_factory)(wct_list);
	if (!scanner->subscribers) {
		wmutex_free(scanner->mutex);
		waolare_free(scanner->resolver);
		waoladi_free(scanner->discoverer);
		waolava_free(scanner->vault);
		wstatus_free(scanner->status);
		wthread_counter_free(scanner->threadCounter);
		wfree_s(scanner);
		scanner = NULL;
		wlog_if_level(wll_warning, "Unable to make scanner subscribers list\n");
		goto end;
	}

	scanner->tasks = wt_idle;
	scanner->asyncDiscoveryCb.caller = scanner;
	scanner->asyncDiscoveryCb.cb = NULL;

	if (state_cbi) {
		waolasc_subscribe_state_s(scanner, state_cbi);
	}

	if (vault_cbi) {
		waolasc_subscribe_vault_s(scanner, vault_cbi);
	}

end:
	return scanner;
}

void waolasc_free_s(waolasc_t* self)
{
	wcont4r_free_s(self->subscribers);
	waolare_free(self->resolver);
	waoladi_free(self->discoverer);
	waolava_free(self->vault);
	wstatus_free(self->status);
	wthread_counter_free(self->threadCounter);
	wmutex_free(self->mutex);
	wfree_s(self);
}

wtask_t waolasc_get_current_tasks_s(const waolasc_t* self)
{
	return self->tasks;
}

BOOL waolasc_is_empty_s(const waolasc_t* self)
{
	return waolava_is_empty(self->vault);
}

BOOL waolasc_subscribe_state_s(waolasc_t* self,
	const waolasc_subscribe_data_t* sd)
{
	BOOL lckd_n_sbscrbd = wmutex_try_lock(self->mutex);

	if (lckd_n_sbscrbd) {
		lckd_n_sbscrbd = merge_subscriber(self, sd);
		wmutex_release(self->mutex);
	}
	else {
		wlog_if_level(wll_warning, "%s: unable to lock mutex\n",
			__func__);
	}

	return lckd_n_sbscrbd;
}

BOOL waolasc_unsubscribe_state_s(waolasc_t* self,
	const waolasc_subscribe_data_t* cbi)
{
	BOOL lckd = wmutex_try_lock(self->mutex);

	if (lckd) {
		wcont4r_delete_s(self->subscribers, cbi);
		wmutex_release(self->mutex);
	}
	else {
		wlog_if_level(wll_warning, "%s: unable to lock mutex\n",
			__func__);
	}

	return lckd;
}

BOOL waolasc_subscribe_vault_s(waolasc_t* self,
	const waolava_subscribe_data_t* sd)
{
	return waolava_subscribe(self->vault, sd);
}

BOOL waolasc_unsubscribe_vault_s(waolasc_t* self,
	const waolava_subscribe_data_t* sd)
{
	return waolava_unsubscribe(self->vault, sd);
}

void waolasc_set_max_threads_s(waolasc_t* self, uint32_t max_threads)
{
	wthread_counter_set_max_threads(self->threadCounter, max_threads);
}

int waolasc_discover_s(waolasc_t* self)
{
	return do_discover(self, FALSE);
}

int waolasc_discover_async_s(waolasc_t* self)
{
	return do_discover(self, TRUE);
}

static int do_discover(waolasc_t* self, BOOL async)
{
	int rc = 0;

	wlog_if_level(wll_info, "Starting network discovery\n");

	if (!register_task(self, wt_discover)) {
		wlog_if_level(wll_info, "%s: unable to register task\n",
			__func__);
		rc = EBUSY;
		goto end;
	}

	wstatus_set(self->status, wm_scanner, wco_discovering, NULL);

	if (async) {
		self->asyncDiscoveryCb.cb = on_discovery_finished;
	}

	rc = waoladi_discover(self->discoverer,
		async ? &self->asyncDiscoveryCb : NULL);

	if (!async) {
		int inner_rc = on_discovery_finished(self);
		if (inner_rc && !rc) {
			rc = inner_rc;
		}
	}

end:
	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}

static int on_discovery_finished(void* const restrict context)
{
	waolasc_t* self = context;

	if (!unregister_task(self, wt_discover)) {
		wlog_if_level(wll_info, "%s: unable to unregister task\n",
			__func__);
	}

	wlog_if_level(wll_info, "Network discovery finished\n");

	return 0;
}

int waolasc_refresh_s(waolasc_t* self)
{
	return do_refresh(self, FALSE);
}

int waolasc_refresh_async_s(waolasc_t* self)
{
	return do_refresh(self, TRUE);
}

static int do_refresh(waolasc_t* self, BOOL async)
{
	int rc = 0;

	wlog_if_level(wll_info, "Starting host(s) refreshing\n");

	if (!register_task(self, wt_refresh)) {
		wlog_if_level(wll_info, "%s: unable to register task\n",
			__func__);
		rc = EBUSY;
		goto end;
	}

	wstatus_set(self->status, wm_scanner, wco_resolving, NULL);

	const wcont4r_t* host_list = waolava_get_host_view_list(self->vault);

	if (async) {
		self->asyncDiscoveryCb.cb = on_refresh_finished;
	}

	rc = waolare_refresh(self->resolver, host_list,
		async ? &self->asyncDiscoveryCb : NULL);

	if (!async || rc) {
		int inner_rc = on_refresh_finished(self);

		if (inner_rc && !rc) {
			rc = inner_rc;
		}
	}

end:
	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}

static int on_refresh_finished(void* const restrict context)
{
	waolasc_t* self = context;

	if (!unregister_task(self, wt_refresh)) {
		wlog_if_level(wll_info, "%s: unable to unregister task\n",
			__func__);
	}

	wlog_if_level(wll_info, "Finished host(s) refreshing\n");

	return 0;
}

void waolasc_refresh_host_s(waolasc_t* self, const whost_view_t* const restrict hw)
{
	wlog_if_level(wll_info, "Starting refreshing of %s\n",
		whost_view_get_host_id_s(hw));

	if (!register_task(self, wt_refresh)) {
		wlog_if_level(wll_info, "%s: unable to register task\n",
			__func__);
		return;
	}

	wstatus_set(self->status, wm_scanner, wco_resolving, NULL);

	waolare_cancel_task(self->resolver);
	waolare_refresh_host(self->resolver, hw);

	if (!unregister_task(self, wt_refresh)) {
		wlog_if_level(wll_warning, "%s: unable to unregister task\n",
			__func__);
	}

	wlog_if_level(wll_info, "Finished host(s) refreshing\n");
}

void waolasc_add_s(waolasc_t* self, const whost_data_t* const restrict hd)
{
	whost_view_t* hw = waolava_add(self->vault, hd);

	if (hw) {
		waolasc_refresh_host_s(self, hw);
	}
}

void waolasc_batch_add_s(waolasc_t* self,
	whost_data_t* hosts[], size_t count)
{
	waolava_batch_add(self->vault, hosts, count);
}

void waolasc_delete_host_s(waolasc_t* self, whost_view_t* const restrict hw)
{
	waolava_delete_host(self->vault, hw);
}

void waolasc_cancel_task_s(waolasc_t* self)
{
	if (wt_idle == self->tasks) {
		return;
	}

	wstatus_set(self->status, wm_scanner,
		wco_cancelling, NULL);

	if (wt_discover & self->tasks) {
		waoladi_cancel_task_s(self->discoverer);
	}

	if (wt_refresh & self->tasks) {
		waolare_cancel_task(self->resolver);
	}
}

const void* waolasc_get_status_s(const waolasc_t* self,
	wmodule_t* module, int* op_code)
{
	return wstatus_get(self->status, module, op_code);
}

const wcont4r_t* waolasc_get_host_list_s(const waolasc_t* self)
{
	return waolava_get_host_view_list(self->vault);
}

static BOOL merge_subscriber(const waolasc_t* self,
	const waolasc_subscribe_data_t* new_sd)
{
	BOOL merged = FALSE;

	waolasc_subscribe_data_t* sd = wcont4r_get_first_s(self->subscribers);
	while (sd) {
		if (sd == new_sd) {
			break;
		}
		sd = wcont4r_get_next_s(self->subscribers);
	}

	if (!sd) {
		merged = wcont4r_add_s(self->subscribers,
			(waolasc_subscribe_data_t*)new_sd);
	}

	return merged;
}

static BOOL register_task(waolasc_t* self, wtask_t task)
{
	BOOL lckd = wmutex_try_lock(self->mutex);
	if (!lckd) {
		wlog_if_level(wll_warning, "%s: couldn't lock mutex\n",
			__func__);
		wstatus_set(self->status, wm_scanner, wco_idle, NULL);
		return FALSE;
	}

	BOOL idle = wt_idle == self->tasks;

	if (idle) {
		self->tasks |= task;
	}

	wmutex_release(self->mutex);

	if (idle) {
		notify_subscribers(self);
	}
	else {
		wlog_if_level(wll_info, "Already running %d; return\n",
			self->tasks);
		return FALSE;
	}

	return TRUE;
}

static BOOL unregister_task(waolasc_t* self, wtask_t task)
{
	BOOL lckd = wmutex_try_lock(self->mutex);
	if (!lckd) {
		wlog_if_level(wll_warning, "%s: couldn't lock mutex\n",
			__func__);
		wstatus_set(self->status, wm_scanner, wco_idle, NULL);
		return FALSE;
	}

	self->tasks &= ~task;

	if (wt_idle == self->tasks) {
		wstatus_set(self->status, wm_scanner, wco_idle, NULL);
	}

	wmutex_release(self->mutex);

	notify_subscribers(self);

	return TRUE;
}

static void notify_subscribers(const waolasc_t* self)
{
	waolasc_cbi_t cbi = {
		.tasks = self->tasks
	};

	BOOL lckd = wmutex_try_lock(self->mutex);

	if (lckd) {
		waolasc_subscribe_data_t* sd = wcont4r_get_first_s(self->subscribers);

		while (sd) {
			cbi.caller = sd->caller;
			(*sd->cb)(&cbi);
			sd = wcont4r_get_next_s(self->subscribers);
		}

		wmutex_release(self->mutex);
	}
	else {
		wlog_if_level(wll_warning, "%s: unable to lock mutex\n",
			__func__);
	}
}
