#include "pch.h"

#include "waolare.h"
#include "warper.h"
#include "wresolver_host.h"

#include "wresolver_hosts_priv.h"

wresolver_hosts_t* wresolver_hosts_make_impl(const waolare_t* const restrict parent)
{
	wresolver_hosts_t* rehs = walloc_s(sizeof(struct wresolver_hosts));

	if (!rehs) {
		wlog_if_level(wll_warning, "No %zuB of free memory for hosts resolver\n",
			sizeof(struct wresolver_hosts));
		goto end;
	}

	const waolare_fact4s_t* fact4s = waolare_get_fact4s(parent);
	rehs->threadCbi = (*fact4s->make_thread_cbi)(rehs, NULL,
		NULL, child_thread_finished_callback, NULL);

	if (!rehs->threadCbi) {
		wresolver_hosts_free(rehs);
		rehs = NULL;
		wlog_if_level(wll_warning, "Unable to make hosts hosts resolver thread callback info\n");
		goto end;
	}

	rehs->parent = parent;
	rehs->threadCounter = waolare_get_thread_counter(parent);
	rehs->status = waolare_get_status(parent);
	rehs->resolvers = NULL;
	rehs->isAsync = waolare_get_is_async(parent);
	rehs->cancelled = FALSE;
	rehs->finishing = FALSE;

end:
	return rehs;
}

int wresolver_hosts_free(wresolver_hosts_t* const self)
{
	if (!self->isAsync && !self->finishing) {
		wresolver_hosts_on_resolving_finished((wresolver_hosts_t*)self);
	}
	
	wthread_cbi_free(self->threadCbi);
	wfree_s(self);
	return 0;
}

int wresolver_hosts_resolve(wresolver_hosts_t* self,
	const wcont4r_t* host_list)
{
	int rc = waola_prepare();

	if (rc) {
		goto end;
	}

	const waolare_fact4s_t* fact4s = waolare_get_fact4s(self->parent);
	self->resolvers = (fact4s->make_cont4r)(wct_list);

	if (!self->resolvers) {
		wlog_if_level(wll_warning, "Unable to make host resolver list\n");
		rc = ENOMEM;
		goto end;
	}

	const waolava_on_host_modified_cbi_t* host_modified_cbi = waolare_get_host_modified_cbi(self->parent);
	wresolver_host_t* reh = NULL;

	whost_view_t* hw = wcont4r_get_first_s(host_list);
	while (hw && !self->cancelled) {
		for (int i = 0; !reh && i < WDEF_TRY_COUNT; ++i) {
			BOOL increased = wresolver_hosts_increment_thread_count(self);
			if (increased) {
				reh = (*fact4s->wmake_resolver_host)(self->threadCbi,
					hw, host_modified_cbi, self->status);
				if (!reh) {
					wresolver_hosts_decrement_thread_count(self);
				}
			}
			else {
				wlog_if_level(wll_warning, "Skipped creating host resolver due to lack of threads\n");
			}

			if (self->cancelled) {
				break;
			}

			if (!reh) {
				wlog_if_level(wll_info, "Couldn't create host resolver, will wait for %u ms\n",
					WDEF_WAIT_TIME_MS << i);
				wthread_sleep(WDEF_WAIT_TIME_MS << i);
			}
		}

		if (reh) {
			wcont4r_add_s(self->resolvers, reh);
			reh = NULL;
		}
		else {
			if (self->cancelled) {
				wlog_if_level(wll_info, "%s: cancelled\n", __func__);
				rc = ECANCELED;
			}
			else {
				wlog_if_level(wll_warning, "Unable to create resolver for '%s' after %d tries\n",
					whost_view_get_host_id_s(hw), WDEF_TRY_COUNT);
				if (0 == rc) {
					rc = ENOMEM;
				}
			}
		}

		hw = wcont4r_get_next_s(host_list);
	}

	if (!self->isAsync || wcont4r_is_empty_s(self->resolvers)) {
		wresolver_hosts_on_resolving_finished(self);
	}

end:
	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}

const waolare_fact4s_t* wresolver_hosts_get_fact4s(const wresolver_hosts_t* self)
{
	return waolare_get_fact4s(self->parent);
}

void wresolver_hosts_cancel(wresolver_hosts_t* self)
{
	self->cancelled = TRUE;
}

BOOL wresolver_hosts_is_cancelled(const wresolver_hosts_t* self)
{
	return self->cancelled;
}

BOOL wresolver_hosts_increment_thread_count(wresolver_hosts_t* self)
{
	return wthread_counter_increase_thread_count(self->threadCounter);
}

void wresolver_hosts_decrement_thread_count(wresolver_hosts_t* self)
{
	wthread_counter_decrease_thread_count(self->threadCounter);
}

static void child_thread_finished_callback(const wthread_cbi_t* const cbi)
{
	const wresolver_hosts_t* self = cbi->threadOwner;
	wthread_counter_thread_finished_callback(self->threadCounter);
	if (self->isAsync && !self->finishing
		&& wthread_counter_get_thread_count(self->threadCounter) == 0) {
		wresolver_hosts_on_resolving_finished((wresolver_hosts_t*)self);
	}
}

wnet_iface_prov_t* wresolver_hosts_get_iface_prov(const wresolver_hosts_t* self)
{
	return waolare_get_iface_prov(self->parent);
}

wnet_iface_t* wresolver_hosts_find_network_iface(const wresolver_hosts_t* self,
	uint32_t ip_addr)
{
	wnet_iface_prov_t* nip = waolare_get_iface_prov(self->parent);
	return wnet_iface_prov_find_iface(nip, ip_addr);
}
