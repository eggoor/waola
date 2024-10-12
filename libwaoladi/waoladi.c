/** @file waoladi.h
*
* Waola network scanner (aka discoverer) implementation.
*/

#include "pch.h"

#include "waoladi_fact4s.h"
#include "wdiscoverer_net.h"

#include "waoladi.h"

struct waoladi
{
	const async_operation_data_t* asyncCbi;
	const waolava_merge_host_cbi_t* mergeHostCbi;
	waoladi_fact4s_t* fact4s;
	wmutex_t* mutex;
	wdiscoverer_net_t* discoverer;
	wthread_counter_t* threadCounter;
	waolast_t* status;
	wnet_iface_prov_t* ifaceProv;
};

waoladi_t* waoladi_make_impl(const wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const wevent_factory event_factory,
	const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory,
	const wnet_iface_prov_factory net_iface_prov_factory,
	const whost_factory host_factory,
	const wdiscoverer_net_factory discoverer_net_factory,
	const wdiscoverer_iface_factory discoverer_iface_factory,
	const wdiscoverer_iface_monitor_factory discoverer_iface_monitor_factory,
	const wdiscoverer_host_factory discoverer_host_factory,
	wthread_counter_t* tc, waolast_t* status,
	const waolava_merge_host_cbi_t* merge_host_cbi)
{
	waoladi_t* di = walloc_s(sizeof(struct waoladi));
	if (!di) {
		wlog_if_level(wll_warning, "No %zuB of free memory for Waola discoverer\n",
			sizeof(struct waoladi));
		goto end;
	}

	di->fact4s = waoladi_fact4s_make(cont4r_factory, mutex_factory, event_factory,
		thread_cbi_factory, thread_factory, net_iface_factory,
		net_iface_prov_factory, host_factory, discoverer_net_factory,
		discoverer_iface_factory, discoverer_iface_monitor_factory,
		discoverer_host_factory);
	if (!di->fact4s) {
		wfree_s(di);
		di = NULL;
		wlog_if_level(wll_warning, "Unable to make Waola discoverer fact4s\n");
		goto end;
	}

	di->mutex = (*di->fact4s->make_mutex)();
	if (!di->mutex) {
		waoladi_fact4s_free(di->fact4s);
		wfree_s(di);
		di = NULL;
		wlog_if_level(wll_warning, "Unable to make discoverer mutex\n");
		goto end;
	}

	di->asyncCbi = NULL;
	di->discoverer = NULL;
	di->threadCounter = tc;
	di->status = status;
	di->mergeHostCbi = merge_host_cbi;
	di->ifaceProv = NULL;

end:
	return di;
}

void waoladi_free(waoladi_t* const self)
{
	if (self->ifaceProv) {
		wnet_iface_prov_free(self->ifaceProv);
	}
	waoladi_fact4s_free(self->fact4s);
	wmutex_free(self->mutex);
	wfree_s(self);
}

wnet_iface_prov_t* waoladi_get_iface_prov(const waoladi_t* self)
{
	return self->ifaceProv;
}

int waoladi_discover(waoladi_t* self,
	const async_operation_data_t* const restrict async_cbi)
{
	int rc = 0;

	BOOL lckd = wmutex_lock(self->mutex, INFINITE);

	if (!lckd) {
		wlog_if_level(wll_error, "Discoverer can't lock mutex\n");
		rc = EBUSY;
		goto end;
	}

	self->ifaceProv = (* self->fact4s->make_net_iface_prov)(self->fact4s->make_cont4r,
		self->fact4s->make_mutex, self->fact4s->make_thread_cbi, self->fact4s->make_thread,
		self->fact4s->make_net_iface, self->threadCounter);

	if (!self->ifaceProv) {
		wmutex_release(self->mutex);
		wlog_if_level(wll_warning, "Unable to make network interface provider\n");
		rc = ENOMEM;
		goto end;
	}

	wnet_iface_prov_prepare_for_arp(self->ifaceProv);

	self->discoverer = (* self->fact4s->make_discoverer_net)(self);

	if (!self->discoverer) {
		wnet_iface_prov_free(self->ifaceProv);
		self->ifaceProv = NULL;
		wmutex_release(self->mutex);
		wlog_if_level(wll_warning, "Unable to make network discoverer\n");
		rc = ENOMEM;
		goto end;
	}

	self->asyncCbi = async_cbi;

	wmutex_release(self->mutex);

	rc = wdiscoverer_net_discover(self->discoverer, !!self->asyncCbi);

	if (!async_cbi) {
		wnet_iface_prov_wait_for_listening_finished(self->ifaceProv);
		int inner_rc = waoladi_on_discovery_finished(self, rc);
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

int waoladi_on_discovery_finished(void* const restrict context, int rc)
{
	waoladi_t* self = context;

	BOOL lckd = wmutex_try_lock(self->mutex);
	if (!lckd) {
		wlog_if_level(wll_warning, "%s: Discoverer can't lock mutex\n",
			__func__);
		return -3;
	}

	if (rc || waolava_is_empty(self->mergeHostCbi->vault)) {
		rc = wdiscoverer_net_get_cached_hosts(self->discoverer);
	}

	if (self->ifaceProv) {
		wnet_iface_prov_free(self->ifaceProv);
		self->ifaceProv = NULL;
	}

	wdiscoverer_net_free_s(self->discoverer);
	self->discoverer = NULL;

	wmutex_release(self->mutex);

	if (self->asyncCbi) {
		(* self->asyncCbi->cb)(self->asyncCbi->caller);
	}

	return rc;
}

void waoladi_cancel_task_s(waoladi_t* self)
{
	if (self->discoverer) {
		wdiscoverer_net_cancel(self->discoverer);
	}
}

const waoladi_fact4s_t* waoladi_get_fact4s(const waoladi_t* self)
{
	return self->fact4s;
}

const waolava_merge_host_cbi_t* waoladi_get_merge_host_cbi(const waoladi_t* self)
{
	return self->mergeHostCbi;
}

wthread_counter_t* waoladi_get_thread_counter(const waoladi_t* self)
{
	return self->threadCounter;
}

waolast_t* waoladi_get_status(const waoladi_t* self)
{
	return self->status;
}
