#include "pch.h"

#include "include/wthread_cbi.h"

wthread_cbi_t* wthread_cbi_make_impl(void* thread_owner, wthreadCb thread_cb,
	wthreadFinishedCb finished_cb, wthreadFinishedCb child_finished_cb,
	const void* extra_data)
{
	wthread_cbi_t* cbi = walloc_s(sizeof(struct wthread_cbi));

	if (!cbi) {
		wlog_if_level(wll_warning, "No %zuB of free memory for thread callbak info\n",
			sizeof(struct wthread_cbi));
		goto end;
	}

	cbi->threadOwner = thread_owner;
	cbi->threadCb = thread_cb;
	cbi->threadFinishedCb = finished_cb;
	cbi->childThreadFinishedCb = child_finished_cb;
	cbi->extraData = extra_data;
	cbi->errNo = 0;

end:
	return cbi;
}

void wthread_cbi_free(wthread_cbi_t* const restrict cbi)
{
	wfree_s(cbi);
}
