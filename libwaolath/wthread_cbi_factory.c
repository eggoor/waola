#include "pch.h"

#include <wthread_cbi_factory.h>

wthread_cbi_t* wthread_cbi_make_impl(void* thread_owner,
	wthreadCb thread_cb, wthreadFinishedCb finished_cb,
	wthreadFinishedCb child_finished_cb, const void* const restrict extra_data);

wthread_cbi_t* wmake_thread_cbi(void* thread_owner,
	wthreadCb thread_cb, wthreadFinishedCb finished_cb,
	wthreadFinishedCb child_finished_cb, const void* const restrict extra_data)
{
	return wthread_cbi_make_impl(thread_owner, thread_cb,
		finished_cb, child_finished_cb, extra_data);
}
