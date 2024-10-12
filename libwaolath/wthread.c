#include "pch.h"

#include "wthread.h"

struct wthread
{
	void* thimpl;
};

wthread_t* wthread_make_impl(wthread_cbi_t* const restrict cbi)
{
	wthread_t* thread = walloc_s(sizeof(struct wthread));

	if (!thread) {
		wlog_if_level(wll_warning, "No %zuB of free memory for wthread\n",
			sizeof(struct wthread));
		goto end;
	}

	wthread_do_make_impl(&thread->thimpl, cbi);

	if (!thread->thimpl) {
		wfree_s(thread);
		thread = NULL;
	}

end:
	return thread;
}

int wthread_join(wthread_t* const restrict thread)
{
	return wthread_do_join_impl(thread->thimpl);
}

int wthread_free(wthread_t* const restrict thread, int timeout_ms)
{
	int rc = wthread_do_free_impl(thread->thimpl, timeout_ms);
	wfree_s(thread);
	return rc;
}
