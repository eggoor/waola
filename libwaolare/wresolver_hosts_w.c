#include "pch.h"

#include "waolare.h"

#include "wresolver_hosts.h"

int wresolver_hosts_on_resolving_finished(wresolver_hosts_t* self)
{
	int rc = 0;

	self->finishing = TRUE;

	wresolver_host_t* reh = wcont4r_get_first_s(self->resolvers);

	while (reh) {
		wresolver_host_free(reh, !self->cancelled ? !self->isAsync : FALSE);
		reh = wcont4r_get_next_s(self->resolvers);
	}

	wcont4r_free_s(self->resolvers);
	self->resolvers = NULL;

	rc = waola_finish();

	if (self->isAsync) {
		waolare_on_resolving_finished((waolare_t*)self->parent);
	}

	self->finishing = FALSE;

	wlog_if_level(rc ? wll_warning : wll_debug,
		"%s exiting with '%s' (%d)\n",
		__func__, wstrerror_s(rc), rc);

	return rc;
}

void wresolver_hosts_query_arp(wresolver_hosts_t* self, const whost_view_t* const restrict hw)
{
}

void wresolver_hosts_on_arp_response(warper_cb_data_t* cb_data)
{
}
