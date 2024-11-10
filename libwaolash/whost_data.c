#include "pch.h"

#include "include/libwaolash.h"

#include "whost_data.h"

whost_data_t* whost_data_make_s(const char* displayName,
	const char* hostName, const char* ipAddr,
	const char* macAddr, time_t lso)
{
	whost_data_t* hd = walloc_s(sizeof(struct whost_data));

	if (!hd) {
		wlog_if_level(wll_warning, "No %zuB of free memory for host data\n",
			sizeof(struct whost_data));
		goto end;
	}

	hd->displayName = wstrdup_s(displayName);
	hd->hostName = wstrdup_s(hostName);
	hd->ipAddr = wstrdup_s(ipAddr);
	hd->macAddr = wstrdup_s(macAddr);
	hd->lastSeenOnline = lso;

end:
	return hd;
}

void whost_data_free_s(whost_data_t* const restrict hd)
{
	wfree_s(hd->displayName);
	wfree_s(hd->hostName);
	wfree_s(hd->ipAddr);
	wfree_s(hd->macAddr);
	wfree_s(hd);
}
