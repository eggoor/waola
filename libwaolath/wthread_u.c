#include "pch.h"

#include "include/wthread.h"

#include "wthread.h"

void wthread_do_make_impl(void** ppt, wthread_cbi_t* const cbi)
{
	if (!ppt) {
		wlog_if_level(wll_error, "No thread pointer provided\n");
		return;
	}

	*ppt = walloc_s(sizeof(pthread_t));

	if (!*ppt) {
		wlog_if_level(wll_error, "No %zuB of free memory for pthread_t\n",
						 sizeof(pthread_t));
		return;		
	}

	int rc = pthread_create((pthread_t*)*ppt, NULL,
		wthread_thread_func, cbi);
	
	if (0 != rc) {
		wfree_s(*ppt);
		*ppt = NULL;
		wlog_if_level(wll_error, "pthread_create failed:: %s (%d)\n",
			strerror(errno), errno);
	}
}

int wthread_do_join_impl(void* const t)
{
	void* thread_return;
	int rc = pthread_join(*(pthread_t*)t, &thread_return);
	if (0 != rc) {
		wlog_if_level(wll_warning, "pthread_join returned %s (%d)\n",
			strerror(rc), rc);
	}
	
	if (0 == rc && 0 != ((wthread_cbi_t*)thread_return)->errNo) {
		rc = ((wthread_cbi_t*)thread_return)->errNo;
	}
	
	return rc;
}

int wthread_do_free_impl(void* const t, int timeout_ms)
{
	int rc = 0;
	
	if (0 != timeout_ms) {
		rc = wthread_do_join_impl(t);
	}
	
	wfree_s(t);
	
	return rc;
}

void wthread_sleep(int32_t ms)
{
	usleep(ms * 1000);
}

void* wthread_thread_func(void* const cbo)
{
	wthread_cbi_t* cbi = cbo;

	int rc = (*cbi->threadCb)(cbi);
	
	if (0 != rc) {
		cbi->errNo = rc;
		
	} else if (0 != errno && ETIMEDOUT != errno) {
		cbi->errNo = errno;
	}

	if (cbi->threadFinishedCb) {
		(*cbi->threadFinishedCb)(cbi);
	}

	return cbo;
}
