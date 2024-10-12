#include "pch.h"

#include "include/wmutex.h"

BOOL wmutex_lock_impl(const void* m, int ms)
{
	int rc = 0;

	if (INFINITE == ms) {
		rc = pthread_mutex_lock((pthread_mutex_t*)m);
	}
	else {
		struct timespec abs_timeout;
		timeout_to_abs_timeout(ms, &abs_timeout);
		rc = pthread_mutex_timedlock((pthread_mutex_t*)m, &abs_timeout);
	}

	return 0 == rc;
}
