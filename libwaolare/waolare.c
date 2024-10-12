/** @file waolare.c
* Waola host resolver implementation.
*/

#include "pch.h"

#include "waolare_fact4s.h"
#include "wresolver_hosts.h"

#include "waolare.h"

struct waolare
{
	const async_operation_data_t* asyncCbi;
	const waolava_on_host_modified_cbi_t* hostModifiedCbi;
	waolare_fact4s_t* fact4s;
	wmutex_t* mutex;
	wthread_counter_t* threadCounter;
	wresolver_hosts_t* resolver;
	waolast_t* status;
	wnet_iface_prov_t* ifaceProv;
};

waolare_t* waolare_make_impl(wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const wevent_factory event_factory,
	const wthread_cbi_factory thread_cbi_factory,
	const wthread_factory thread_factory,
	const wnet_iface_factory net_iface_factory,
	const wnet_iface_prov_factory net_iface_prov_factory,
	const wresolver_hosts_factory resolver_hosts_factory,
	const wresolver_host_factory resolver_host_factory,
	wthread_counter_t* tc, waolast_t* status,
	const waolava_on_host_modified_cbi_t* hostModifiedCbi)
{
	waolare_t* re = walloc_s(sizeof(struct waolare));
	if (!re) {
		wlog_if_level(wll_warning, "No %zuB of free memory for Waola host resolver\n",
			sizeof(struct waolare));
		goto end;
	}

	re->fact4s = wmake_waolare_fact4s(cont4r_factory, mutex_factory,
		thread_cbi_factory,	thread_factory, net_iface_factory,
		net_iface_prov_factory, resolver_hosts_factory, resolver_host_factory);
	if (!re->fact4s) {
		wfree_s(re);
		re = NULL;
		wlog_if_level(wll_warning, "Unable to make Waola host resolver factories\n");
		goto end;
	}

	re->mutex = (*mutex_factory)();
	if (!re->mutex) {
		waolare_fact4s_free(re->fact4s);
		wfree_s(re);
		re = NULL;
		wlog_if_level(wll_warning, "Unable to make Waola host resolver mutex\n");
		goto end;
	}

	re->asyncCbi = NULL;
	re->resolver = NULL;
	re->ifaceProv = NULL;
	re->threadCounter = tc;
	re->status = status;
	re->hostModifiedCbi = hostModifiedCbi;

end:
	return re;
}

void waolare_free(waolare_t* const self)
{
	if (self->ifaceProv) {
		wnet_iface_prov_free(self->ifaceProv);
	}
	wmutex_free(self->mutex);
	waolare_fact4s_free(self->fact4s);
	wfree_s(self);
}

const waolare_fact4s_t* waolare_get_fact4s(const waolare_t* self)
{
	return self->fact4s;
}

wthread_counter_t* waolare_get_thread_counter(const waolare_t* self)
{
	return self->threadCounter;
}

waolast_t* waolare_get_status(const waolare_t* self)
{
	return self->status;
}

wnet_iface_prov_t* waolare_get_iface_prov(const waolare_t* self)
{
	return self->ifaceProv;
}

const waolava_on_host_modified_cbi_t* waolare_get_host_modified_cbi(const waolare_t* self)
{
	return self->hostModifiedCbi;
}

BOOL waolare_get_is_async(const waolare_t* self)
{
	return !!self->asyncCbi;
}

int waolare_refresh(waolare_t* self, const wcont4r_t* host_list,
	const async_operation_data_t* const restrict async_cbi)
{
	int rc = 0;

	BOOL lckd = wmutex_try_lock(self->mutex);

	if (!lckd) {
		wlog_if_level(wll_error, "Resolver can't lock mutex\n");
		rc = EBUSY;
		goto end;
	}

	self->asyncCbi = async_cbi;

	self->ifaceProv = (* self->fact4s->make_net_iface_prov)(self->fact4s->make_cont4r,
		self->fact4s->make_mutex, self->fact4s->make_thread_cbi, self->fact4s->make_thread,
		self->fact4s->make_net_iface, self->threadCounter);

	if (!self->ifaceProv) {
		wlog_if_level(wll_warning, "Unable to make network interface provider\n");
		rc = ENOMEM;
		goto end;
	}
	
	self->resolver = (* self->fact4s->wmake_resolver_hosts)(self);

	if (!self->resolver) {
		wlog_if_level(wll_warning, "Unable to make a resolver\n");
		waolare_on_resolving_finished(self);
		rc = ENOMEM;
		goto end;
	}
	
	if (!wnet_iface_prov_prepare_for_arp(self->ifaceProv)) {
		wlog_if_level(wll_warning, "Preparation for ARP failed for all collected network interfaces\n");
	}

	rc = wresolver_hosts_resolve(self->resolver, host_list);

	if (!async_cbi) {
		waolare_on_resolving_finished(self);
	}

end:
	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}

void waolare_on_resolving_finished(waolare_t* self)
{
	wresolver_hosts_free(self->resolver);
	self->resolver = NULL;
	
	wnet_iface_prov_free(self->ifaceProv);
	self->ifaceProv = NULL;
	
	wmutex_release(self->mutex);
	
	if (self->asyncCbi) {
		(* self->asyncCbi->cb)(self->asyncCbi->caller);
	}
}

void waolare_refresh_host(waolare_t* const restrict re,
	const whost_view_t* const restrict hw)
{
	wcont4r_t* host_list = (*re->fact4s->make_cont4r)(wct_list);

	if (!host_list) {
		wlog_if_level(wll_warning, "Unable to make resolver's host view list\n");
		return;
	}

	wcont4r_add_s(host_list, (whost_view_t*)hw);
	waolare_refresh(re, host_list, NULL);
	wcont4r_free_s(host_list);
}

void waolare_cancel_task(waolare_t* const restrict re)
{
	if (re->resolver) {
		wresolver_hosts_cancel(re->resolver);
		wlog_if_level(wll_info, "Refreshing cancelled\n");
	}
}
