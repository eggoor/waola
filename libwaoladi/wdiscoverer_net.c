#include "pch.h"

#include "waoladi.h"

#include "wdiscoverer_iface.h"

#include "wdiscoverer_net_priv.h"

struct wdiscoverer_net {
#if defined(DEBUG) || defined(_DEBUG)
	const char* id;
#endif
	const waoladi_t* parent;
	wthread_counter_t* threadCounter;
	wthread_cbi_t* threadCbi;
	wcont4r_t* discoverers;
	BOOL cancelled;
	BOOL isAsync;
	BOOL lookingArpCache;
	wmutex_t* discoveryScheduledEventMutex;
	wevent_t* discoveryScheduledEvent;
	BOOL allDiscoveriesAreScheduled;
};

wdiscoverer_net_t* wdiscoverer_net_make_impl(waoladi_t* const restrict parent)
{
	wdiscoverer_net_t* drn = walloc_s(sizeof(struct wdiscoverer_net));

	if (!drn) {
		wlog_if_level(wll_warning, "No %zuB of free memory for network discoverer\n",
			sizeof(struct wdiscoverer_net));
		goto end;
	}

	const waoladi_fact4s_t* fact4s = waoladi_get_fact4s(parent);

	drn->threadCbi = (*fact4s->make_thread_cbi)(drn, NULL,
		NULL, child_thread_finished_callback, NULL);

	if (!drn->threadCbi) {
		wfree_s(drn);
		drn = NULL;
		wlog_if_level(wll_warning,
			"Unable to make network discoverer thread callback info\n");
		goto end;
	}

	drn->discoveryScheduledEventMutex = (*fact4s->make_mutex)();
	if (!drn->discoveryScheduledEventMutex) {
		wthread_cbi_free(drn->threadCbi);
		wfree_s(drn);
		drn = NULL;
		wlog_if_level(wll_error, "Unable to make network discoverer event mutex\n");
		goto end;
	}

	drn->discoveryScheduledEvent = (*fact4s->make_event)();
	if (!drn->discoveryScheduledEvent) {
		wmutex_free(drn->discoveryScheduledEventMutex);
		wthread_cbi_free(drn->threadCbi);
		wfree_s(drn);
		drn = NULL;
		wlog_if_level(wll_error, "Unable to make network discoverer event\n");
		goto end;
	}

	drn->parent = parent;
	drn->threadCounter = waoladi_get_thread_counter(parent);
	drn->cancelled = FALSE;
	drn->discoverers = NULL;
	drn->isAsync = FALSE;
	drn->lookingArpCache = FALSE;
	drn->allDiscoveriesAreScheduled = FALSE;

#if defined(DEBUG) || defined(_DEBUG)
	drn->id = "Network discoverer";
	wstrncpy_s(drn->threadCbi->ownerId, THR_CBI_ID_BUF_SIZE,
		drn->id, WMIN(THR_CBI_ID_MAX_LEN, strlen(drn->id)));
#endif

end:
	return drn;
}

void wdiscoverer_net_free_s(wdiscoverer_net_t* self)
{
	wevent_free(self->discoveryScheduledEvent);
	wmutex_free(self->discoveryScheduledEventMutex);
	wthread_cbi_free(self->threadCbi);
	wfree_s(self);
}

int wdiscoverer_net_discover(wdiscoverer_net_t* self, BOOL async)
{
	int rc = 0;

	const waoladi_fact4s_t* fact4s = waoladi_get_fact4s(self->parent);
	self->discoverers = (*fact4s->make_cont4r)(wct_list);

	if (!self->discoverers) {
		wlog_if_level(wll_warning, "Unable to make interface discoverer list\n");
		rc = ENOMEM;
		goto end;
	}

	rc = waola_prepare();

	if (rc) {
		goto end;
	}

	self->cancelled = FALSE;
	self->isAsync = async;

	const waolava_merge_host_cbi_t* merge_host_cbi = waoladi_get_merge_host_cbi(self->parent);
	wnet_iface_prov_t* iface_prov = waoladi_get_iface_prov(self->parent);
	const wcont4r_t* iface_list = wnet_iface_prov_get_collected_ifaces(iface_prov);

	wnet_iface_t* nif = wcont4r_get_first_s(iface_list);
	while (nif && !self->cancelled) {
		sa_family_t saf = wnet_iface_get_addr_family(nif);

		if (AF_INET == saf) {
			wdiscoverer_iface_t* dri = NULL;
			for (int i = 0; !dri && i < WDEF_TRY_COUNT; ++i) {
				dri = (*fact4s->make_discoverer_iface)(self->threadCbi,
					nif, merge_host_cbi);

				if (!dri) {
					wlog_if_level(wll_info, "Couldn't create network interface discoverer, will wait for %u ms\n",
						WDEF_WAIT_TIME_MS << i);
					wthread_sleep(WDEF_WAIT_TIME_MS << i);
				}

				if (self->cancelled) {
					break;
				}
			}

			if (dri) {
				if (!self->cancelled) {
					BOOL increased = wdiscoverer_net_increment_thread_count(self);
					if (increased) {
						BOOL started = wdiscoverer_iface_discover(dri);
						if (started && !self->cancelled) {
							wcont4r_add_s(self->discoverers, dri);
						}
						else {
							if (!started) {
								wdiscoverer_net_decrement_thread_count(self);
							}
							wdiscoverer_iface_free(dri, FALSE);
						}
					}
					else {
						wlog_if_level(wll_warning, "Skipped inteface discovery due to lack of threads\n");
						wdiscoverer_iface_free(dri, FALSE);
					}
				}
				else {
					wdiscoverer_iface_free(dri, FALSE);
				}
			}
			else if (wlog_get_level() >= wll_warning) {
				if (self->cancelled) {
					wlog_if_level(wll_info, "%s: Discovery task cancelled\n", __func__);
					rc = ECANCELED;
				}
				else {
					wlog(wll_warning, "Unable to make network interface discoverer after %d tries\n",
						WDEF_TRY_COUNT);
					rc = ENOMEM;
				}
			}
		}
		nif = wcont4r_get_next_s(iface_list);
	}

	wmutex_lock(self->discoveryScheduledEventMutex, INFINITE);
	self->allDiscoveriesAreScheduled = TRUE;
	wmutex_release(self->discoveryScheduledEventMutex);

	wevent_set(self->discoveryScheduledEvent);

	if (!self->isAsync || wcont4r_is_empty_s(self->discoverers)) {
		rc = on_discovery_finished(self);
	}

end:
	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}

static int on_discovery_finished(wdiscoverer_net_t* self)
{
	int rc = 0;

	wdiscoverer_iface_t* dri = wcont4r_get_first_s(self->discoverers);

	while (dri)
	{
		int inner_rc = wdiscoverer_iface_free(dri, !self->isAsync);
		if (inner_rc && !rc)
		{
			rc = inner_rc;
		}
		dri = wcont4r_get_next_s(self->discoverers);
	}

	wcont4r_free_s(self->discoverers);
	self->discoverers = NULL;

	if (0 == rc && 0 != self->threadCbi->errNo) {
		rc = self->threadCbi->errNo;
	}

	waola_finish();

	if (self->isAsync) {
		waoladi_on_discovery_finished((waoladi_t*)self->parent, rc);
	}

	return rc;
}

const waoladi_fact4s_t* wdiscoverer_net_get_fact4s(const wdiscoverer_net_t* self)
{
	return waoladi_get_fact4s(self->parent);
}

waolast_t* wdiscoverer_net_get_status(const wdiscoverer_net_t* self)
{
	return waoladi_get_status(self->parent);
}

void wdiscoverer_net_cancel(wdiscoverer_net_t* self)
{
	self->cancelled = TRUE;
	wlog_if_level(wll_info, "Discovery cancelled\n");
}

BOOL wdiscoverer_net_is_task_cancelled(const wdiscoverer_net_t* self)
{
	return self->cancelled;
}

BOOL wdiscoverer_net_increment_thread_count(wdiscoverer_net_t* self)
{
	return wthread_counter_increase_thread_count(self->threadCounter);
}

void wdiscoverer_net_decrement_thread_count(wdiscoverer_net_t* self)
{
	wthread_counter_decrease_thread_count(self->threadCounter);
}

uint32_t wdiscoverer_net_get_thread_count(const wdiscoverer_net_t* self)
{
	return wthread_counter_get_thread_count(self->threadCounter);
}

int wdiscoverer_net_get_cached_hosts(wdiscoverer_net_t* self)
{
	self->lookingArpCache = TRUE;
	wlog_if_level(wll_info, "Getting hosts from ARP cache\n");

	const waoladi_fact4s_t* fact4s = waoladi_get_fact4s(self->parent);
	wnet_iface_prov_t* iface_prov = waoladi_get_iface_prov(self->parent);

	wcont4r_t* drh_list = (*fact4s->make_cont4r)(wct_list);
	if (!drh_list) {
		wlog_if_level(wll_warning, "Unable to make host dicoverer list\n");
		goto end;
	}

	wcont4r_t* dri_list = (*fact4s->make_cont4r)(wct_list);
	if (!dri_list) {
		wcont4r_free_s(drh_list);
		wlog_if_level(wll_warning, "Unable to make host dicoverer list\n");
		goto end;
	}

	const wcont4r_t* arp_cache = wnet_iface_prov_get_cached(iface_prov);

	if (arp_cache) {
		const waolava_merge_host_cbi_t* merge_host_cbi = waoladi_get_merge_host_cbi(self->parent);
		arp_cache_entry_t* ce = wcont4r_get_first_s(arp_cache);
		while (ce) {
			wnet_iface_t* nif = wnet_iface_prov_find_iface(iface_prov, ce->ipAddr);
			if (nif) {
				wdiscoverer_iface_t* dri = NULL;

				wdiscoverer_iface_t* dri_it = wcont4r_get_first_s(dri_list);
				while (dri_it) {
					if (wdiscoverer_iface_get_net_iface(dri_it) == nif) {
						dri = dri_it;
						break;
					}
					dri_it = wcont4r_get_next_s(dri_list);
				}

				for (int i = 0; !dri && i < WDEF_TRY_COUNT; ++i) {
					dri = (*fact4s->make_discoverer_iface)(self->threadCbi,
						nif, merge_host_cbi);

					if (!dri) {
						wlog_if_level(wll_info, "Couldn't create network interface discoverer, will wait for %u ms\n",
							WDEF_WAIT_TIME_MS << i);
						wthread_sleep(WDEF_WAIT_TIME_MS << i);
					}
				}

				if (!dri) {
					wlog_if_level(wll_warning, "Unable to make network interface discoverer after %d tries\n",
						WDEF_TRY_COUNT);
					continue;
				}

				BOOL increased = wdiscoverer_net_increment_thread_count(self);
				if (increased) {
					wdiscoverer_host_t* drh = wdiscoverer_iface_make_host_discoverer(dri,
						ce->ipAddr, ce->macAddr);

					if (drh) {
						wcont4r_add_s(drh_list, drh);
					}
					else {
						wdiscoverer_net_decrement_thread_count(self);
						if (wlog_get_level() >= wll_info) {
							if (wdiscoverer_net_is_task_cancelled(self)) {
								wlog_if_level(wll_info, "Discovery task cancelled\n");
							}
							else {
								wlog(wll_debug, "Unable to make host discoverer\n");
							}
						}

						if (wdiscoverer_net_is_task_cancelled(self)) {
							break;
						}
					}
				}
				else {
					wlog_if_level(wll_warning, "Skipped host discovery due to lack of threads\n");
				}
			}
			else if (wlog_get_level() >= wll_warning) {
				char ip_str[INET_ADDRSTRLEN];
				wsockaddr_int_to_string_s(ce->ipAddr, ip_str, INET_ADDRSTRLEN);
				wlog(wll_warning, "Network interface corresponding to IP address %s not found\n",
					ip_str);
			}

			ce = wcont4r_get_next_s(arp_cache);
		}
	}
	else {
		wlog_if_level(wll_debug, "ARP cache is (null)");
	}

	wdiscoverer_host_t* drh = wcont4r_get_first_s(drh_list);
	while (drh) {
		wdiscoverer_host_free(drh);
		drh = wcont4r_get_next_s(drh_list);
	}
	wcont4r_free_s(drh_list);

	wdiscoverer_iface_t* dri = wcont4r_get_first_s(dri_list);
	while (dri) {
		wdiscoverer_iface_notify_discovery_scheduling_finished(dri);
		wdiscoverer_iface_free(dri, TRUE);
		dri = wcont4r_get_next_s(dri_list);
	}
	wcont4r_free_s(dri_list);

end:
	self->lookingArpCache = FALSE;

	return 0;
}


static void child_thread_finished_callback(const wthread_cbi_t* const cbi)
{
	const wdiscoverer_net_t* self = cbi->threadOwner;
	wthread_counter_thread_finished_callback(self->threadCounter);

	if (self->lookingArpCache) {
		return;
	}

	if (self->isAsync) {
		BOOL allDiscoveriesAreScheduled = FALSE;
		wmutex_lock(self->discoveryScheduledEventMutex, INFINITE);
		allDiscoveriesAreScheduled = self->allDiscoveriesAreScheduled;
		wmutex_release(self->discoveryScheduledEventMutex);

		if (!allDiscoveriesAreScheduled) {
			wevent_wait(self->discoveryScheduledEvent, INFINITE);
		}

		if (!self->cancelled && wthread_counter_get_thread_count(self->threadCounter) == 0) {
			on_discovery_finished((wdiscoverer_net_t*)self);
		}
	}
}

