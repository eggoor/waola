#include "pch.h"

#include "include/wmutex.h"

int wmutex_do_make_impl(void** const ppm)
{
	if (!ppm) {
		wlog_if_level(wll_error, "No mutex pointer provided\n");
		return -1;
	}

	*ppm = walloc_s(sizeof(pthread_mutex_t));

	if (!*ppm) {
		wlog_if_level(wll_warning, "No %zuB for pthread mutex\n",
						sizeof(pthread_mutex_t));
		return -2;
	}

	int rc = pthread_mutex_init(*ppm, NULL);

	if (0 != rc) {
		wlog_if_level(wll_error, "%s: pthread_mutex_init failed with %s (%d)\n",
			__func__, strerror(rc), rc);
		wfree_s(*ppm);
		*ppm = NULL;
	}

	return rc;
}

void wmutex_do_free_impl(void* const m)
{
	pthread_mutex_destroy(m);
	wfree_s(m);
}

BOOL wmutex_try_lock_once_impl(void* const m)
{
	return pthread_mutex_trylock((pthread_mutex_t*)m) == 0;
}

void wmutex_release_impl(void* const m)
{
	pthread_mutex_unlock((pthread_mutex_t*)m);
}
