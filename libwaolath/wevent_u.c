#include "pch.h"

#include "wcond.h"

#include "wevent.h"

int wevent_do_make_impl(void** const ppe)
{
	if (!ppe) {
		wlog_if_level(wll_error, "No event pointer provided\n");
		return -1;
	}

	wcond_t* cond = walloc_s(sizeof(struct wcond));

	if (!cond) {
		wlog_if_level(wll_warning, "No %zuB for struct wcond\n",
						sizeof(struct wcond));
		return -2;
	}

	int rc = pthread_mutex_init(&cond->m, NULL);
	if (0 != rc) {
		wlog_if_level(wll_error, "%s: pthread_mutex_init error %s (%d)",
			__func__, strerror(rc), rc);
		wfree_s(cond);
		cond = NULL;
	}

	if (0 == rc) {
		rc = pthread_cond_init(&cond->cond, NULL);
		if (0 != rc) {
			wlog_if_level(wll_error, "%s: pthread_mutex_init error %s (%d)",
				__func__, strerror(rc), rc);
			wfree_s(cond);
			cond = NULL;
		}
	}

	*ppe = cond;

	return rc;
}

void wevent_do_free_impl(void* const e)
{
	wcond_t* cond = e;
	pthread_cond_destroy(&cond->cond);
	pthread_mutex_destroy(&cond->m);
	wfree_s(cond);
}

BOOL wevent_wait_impl(void* const restrict e, int timeout_ms)
{
	int rc = 0;
	wcond_t* cond = e;

	if (INFINITE == timeout_ms) {
		rc = pthread_cond_wait(&cond->cond, &cond->m);
	}
	else {
		struct timespec abs_timeout;
		timeout_to_abs_timeout(timeout_ms, &abs_timeout);
		rc = pthread_cond_timedwait(&cond->cond, &cond->m, &abs_timeout);
	}

	if (0 != rc) {
		wlog_if_level(wll_warning, "pthread_cond_(timed)wait finifed with %s (%d)\n",
			strerror(rc), rc);
	}
	
	return 0 == rc;
}

void wevent_set_impl(void* const e)
{
	wcond_t* cond = e;
	int rc = pthread_cond_signal(&cond->cond);
	if (0 != rc) {
		wlog_if_level(wll_warning, "pthread_cond_signal finifed with %s (%d)\n",
			strerror(rc), rc);
	}
}
