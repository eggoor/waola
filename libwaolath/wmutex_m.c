#include "pch.h"

#include "include/wthread.h"

#include "include/wmutex.h"

BOOL wmutex_lock_impl(const void* const m, int ms)
{
	int rc = 0;

	if (INFINITE == ms) {
		rc = pthread_mutex_lock((pthread_mutex_t*)m);
	}
	else {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		while ((rc = pthread_mutex_trylock((pthread_mutex_t*)m)) && ms--) {
			wthread_sleep(1);
		}
	}
	
	if (0 != rc) {
		wlog_if_level(wll_error, "pthread_mutex_trylock failed: %s (%d)\n",
						strerror(errno), errno);
	}

	return 0 == rc;
}
