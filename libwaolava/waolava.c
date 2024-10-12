/** @file waolava.h
* Waola vault implementation.
*/

#include "pch.h"

#include "waolava_priv.h"
#include "fact4s.h"

#define MAX_LOCK_HOST_LIST_TRY_COUNT 8
#define MAX_LOCK_SUBSCRIBER_LIST_TRY_COUNT 3

struct waolava
{
	waolava_fact4s_t* fact4s;
	wcont4r_t* hosts;
	wmutex_t* mutexHosts;
	wcont4r_t* subscribers;
	wmutex_t* mutexSubscribers;
	waolava_merge_host_cbi_t* mergeHostCbi;
	waolava_on_host_modified_cbi_t* onHostModifiedCbi;
};

waolava_t* waolava_make_impl(const wcont4r_factory cont4r_factory,
	const wmutex_factory mutex_factory,
	const whost_from_data_factory host_from_data_factory,
	const whost_view_factory host_view_factory,
	const waolava_on_host_modified_cbi_factory waolava_on_host_modified_cbi_factory,
	const waolava_merge_host_cbi_factory waolava_merge_host_cbi_factory)
{
	waolava_t* vault = walloc_s(sizeof(struct waolava));
	if (!vault) {
		wlog_if_level(wll_warning, "No %zuB of free memory for Waola vault\n",
			sizeof(struct waolava));
		goto end;
	}

	vault->fact4s = fact4s_make(host_from_data_factory, host_view_factory);
	if (!vault->fact4s) {
		wfree_s(vault);
		vault = NULL;
		wlog_if_level(wll_warning, "Unable to make Waola vault factories\n");
		goto end;
	}

	vault->mutexHosts = (*mutex_factory)();
	if (!vault->mutexHosts) {
		fact4s_free(vault->fact4s);
		wfree_s(vault);
		vault = NULL;
		wlog_if_level(wll_warning, "Unable to make vault host list mutex\n");
		goto end;
	}

	vault->mutexSubscribers = (*mutex_factory)();
	if (!vault->mutexSubscribers) {
		wmutex_free(vault->mutexHosts);
		fact4s_free(vault->fact4s);
		wfree_s(vault);
		vault = NULL;
		wlog_if_level(wll_warning, "Unable to make vault subscribers mutex\n");
		goto end;
	}

	vault->hosts = (*cont4r_factory)(wct_list);
	if (!vault->hosts) {
		wmutex_free(vault->mutexSubscribers);
		wmutex_free(vault->mutexHosts);
		fact4s_free(vault->fact4s);
		wfree_s(vault);
		vault = NULL;
		wlog_if_level(wll_warning, "Unable to make host container\n");
		goto end;
	}

	vault->subscribers = (*cont4r_factory)(wct_list);
	if (!vault->subscribers) {
		wcont4r_free_s(vault->hosts);
		wmutex_free(vault->mutexSubscribers);
		wmutex_free(vault->mutexHosts);
		fact4s_free(vault->fact4s);
		wfree_s(vault);
		vault = NULL;
		wlog_if_level(wll_warning, "Unable to make subscriber container\n");
		goto end;
	}

	vault->mergeHostCbi = (*waolava_merge_host_cbi_factory)(vault, waolava_merge_host_cb);
	if (!vault->mergeHostCbi) {
		wcont4r_free_s(vault->subscribers);
		wcont4r_free_s(vault->hosts);
		wmutex_free(vault->mutexSubscribers);
		wmutex_free(vault->mutexHosts);
		fact4s_free(vault->fact4s);
		wfree_s(vault);
		vault = NULL;
		wlog_if_level(wll_warning, "Unable to make subscriber container\n");
		goto end;
	}

	vault->onHostModifiedCbi = (*waolava_on_host_modified_cbi_factory)(vault, waolava_on_host_modified_cb);
	if (!vault->mergeHostCbi) {
		waolava_callback_info_free(vault->mergeHostCbi);
		wcont4r_free_s(vault->subscribers);
		wcont4r_free_s(vault->hosts);
		wmutex_free(vault->mutexSubscribers);
		wmutex_free(vault->mutexHosts);
		fact4s_free(vault->fact4s);
		wfree_s(vault);
		vault = NULL;
		wlog_if_level(wll_warning, "Unable to make subscriber container\n");
		goto end;
	}

end:
	return vault;
}

void waolava_free(waolava_t* const self)
{
	waolava_host_modified_cbi_free(self->onHostModifiedCbi);
	waolava_callback_info_free(self->mergeHostCbi);
	free_host_view_list(self);
	wcont4r_free_s(self->subscribers);
	fact4s_free(self->fact4s);
	wmutex_free(self->mutexHosts);
	wmutex_free(self->mutexSubscribers);
	wfree_s(self);
}

const wcont4r_t* waolava_get_host_view_list(const waolava_t* restrict const self)
{
	return self->hosts;
}

void waolava_delete_host(waolava_t* restrict const self, whost_view_t* restrict const hwfd)
{
	if (wcont4r_delete_s(self->hosts, hwfd)) {
		notify_subscribers(self, wva_deleted, hwfd);
		whost_view_free(hwfd);
	}
}

BOOL waolava_is_empty(const waolava_t* restrict const self)
{
	return wcont4r_is_empty_s(self->hosts);
}

BOOL waolava_subscribe(const waolava_t* restrict const self,
	const waolava_subscribe_data_t* sd)
{
	BOOL subscribed = try_lock_subscriber_list(self);

	if (subscribed) {
		subscribed = merge_subscriber(self, sd);
		wmutex_release(self->mutexSubscribers);
	}

	return subscribed;
}

BOOL waolava_unsubscribe(const waolava_t* restrict const self,
	const waolava_subscribe_data_t* sd)
{
	BOOL lckd_n_unsbscrbd = try_lock_subscriber_list(self);

	if (lckd_n_unsbscrbd) {
		lckd_n_unsbscrbd = wcont4r_delete_s(self->subscribers, sd);
		wmutex_release(self->mutexSubscribers);
	}

	return lckd_n_unsbscrbd;
}

whost_view_t* waolava_add(waolava_t* restrict const self, const whost_data_t* restrict const hd)
{
	whost_view_t* hw = NULL;

	if (whost_data_is_empty_s(hd)) {
		wlog_if_level(wll_warning, "Ignore completely empty host data\n");
		goto end;
	}

	whost_t* host = (* self->fact4s->make_host_from_data)(hd);
	if (!host) {
		wlog_if_level(wll_warning, "Unable to make new host\n");
		goto end;
	}

	BOOL acquired = FALSE;
	BOOL new_host = FALSE;
	BOOL lckd = try_lock_host_list(self);

	if (lckd) {
		hw = merge_host(self, host, &acquired, &new_host);
		wmutex_release(self->mutexHosts);

		if (hw && whost_view_is_updated_s(hw)) {
			notify_subscribers(self, new_host ? wva_added : wva_modified, hw);
		}
	}

	if (!acquired) {
		whost_free(host);
	}

end:
	return hw;
}

void waolava_batch_add(waolava_t* restrict const self, whost_data_t* restrict const hosts[],
	size_t count)
{
	for (size_t i = 0; i < count; ++i) {
		waolava_add(self, hosts[i]);
		whost_data_free_s(hosts[i]);
	}
}

BOOL waolava_merge_host_cb(waolava_t* const self, whost_t* const host)
{
	BOOL acquired = FALSE;
	BOOL new_host = FALSE;
	BOOL lckd = try_lock_host_list(self);

	if (lckd) {
		whost_view_t* hw = merge_host(self, host, &acquired, &new_host);
		wmutex_release(self->mutexHosts);

		if (hw && whost_view_is_updated_s(hw)) {
			notify_subscribers(self, new_host ? wva_added : wva_modified, hw);
		}
	}

	return acquired;
}

void waolava_on_host_modified_cb(waolava_t* const self, const whost_view_t* const mhw)
{
	BOOL lckd = try_lock_host_list(self);

	if (lckd) {
		whost_view_t* hw = wcont4r_get_first_s(self->hosts);
		while (hw) {
			if (mhw == hw) {
				break;
			}

			hw = wcont4r_get_next_s(self->hosts);
		}
		wmutex_release(self->mutexHosts);

		if (hw) {
			notify_subscribers(self, wva_modified, hw);
		}
		else {
			wlog_if_level(wll_error, "%s: host view '%p' not found in the vault\n",
				__func__, mhw);
		}
	}
}

static BOOL try_lock_host_list(const waolava_t* restrict const self)
{
	BOOL lckd = FALSE;

	for (int i = 0; !lckd && i < MAX_LOCK_HOST_LIST_TRY_COUNT; ++i) {
		lckd = wmutex_lock(self->mutexHosts,
			DEF_LOCK_TIMEOUT_MS << i);
	}

	if (!lckd) {
		wlog_if_level(wll_error, "Couldn't lock host list mutex\n");
	}

	return lckd;
}

static BOOL try_lock_subscriber_list(const waolava_t* restrict const self)
{
	BOOL lckd = FALSE;

	for (int i = 0; !lckd && i < MAX_LOCK_SUBSCRIBER_LIST_TRY_COUNT; ++i) {
		lckd = wmutex_lock(self->mutexSubscribers,
			DEF_LOCK_TIMEOUT_MS << i);
	}

	if (!lckd) {
		wlog_if_level(wll_warning, "Couldn't lock subscriber list mutex\n");
	}

	return lckd;
}

static whost_view_t* merge_host(const waolava_t* restrict const self,
	whost_t* incoming_host,	BOOL* acquired, BOOL* new_host)
{
	whost_view_t* hw = NULL;

	hw = wcont4r_get_first_s(self->hosts);
	while (hw) {
		const whost_t* stored_host = whost_view_get_host_s(hw);

		if (whost_compare_s(stored_host, incoming_host) == 0) {

			if (stored_host == incoming_host) {
				*acquired = TRUE;
			}
			
			break;
		}

		hw = wcont4r_get_next_s(self->hosts);
	}

	if (!hw) {
		hw = (* self->fact4s->make_host_view)(incoming_host);
		if (hw) {
			*new_host = TRUE;
			*acquired = TRUE;
			wcont4r_add_s(self->hosts, hw);
			wlog_if_level(wll_verbose, "New host %s %s %s is added to the vault\n",
				whost_view_to_string_s(hw), whost_view_get_ip_addr_s(hw),
				whost_view_get_hostname_s(hw));
		}
	}
	else if (!whost_view_update_host_s(hw, incoming_host)) {
		hw = NULL;
	}

	return hw;
}

static BOOL merge_subscriber(const waolava_t* restrict const self,
	const waolava_subscribe_data_t* new_sd)
{
	BOOL merged = FALSE;

	const waolava_subscribe_data_t* sd = wcont4r_get_first_s(self->subscribers);
	while (sd) {
		if (sd == new_sd) {
			break;
		}
		sd = wcont4r_get_next_s(self->subscribers);
	}

	if (!sd) {
		merged = wcont4r_add_s(self->subscribers,
			(waolava_subscribe_data_t*)new_sd);
	}

	return merged;
}

static void free_host_view_list(const waolava_t* restrict const self)
{
	whost_view_t* hw = wcont4r_get_first_s(self->hosts);

	while (hw) {
		whost_view_free(hw);
		hw = wcont4r_get_next_s(self->hosts);
	}

	wcont4r_free_s(self->hosts);
}

void notify_subscribers(const waolava_t* restrict const self,
	waolava_op_t op, whost_view_t* host)
{
	waolava_cbi_t cbi =
	{
		.hostView = host,
		.opCode = op
	};

	const waolava_subscribe_data_t* sd = wcont4r_get_first_s(self->subscribers);
	while (sd) {
		cbi.caller = sd->caller;
		(*sd->cb)(&cbi);
		sd = wcont4r_get_next_s(self->subscribers);
	}
}

const waolava_on_host_modified_cbi_t* waolava_get_host_modidied_cbi(const waolava_t* restrict const self)
{
	return self->onHostModifiedCbi;
}

const waolava_merge_host_cbi_t* waolava_get_merge_host_cbi(const waolava_t* restrict const self)
{
	return self->mergeHostCbi;
}
