#include "pch.h"

void timeout_to_abs_timeout(int ms, struct timespec* const abs_timeout)
{
	clock_gettime(CLOCK_REALTIME, abs_timeout);
	abs_timeout->tv_sec += ms / 1000;
	abs_timeout->tv_nsec += ms % 1000 * 1000000;

	if (abs_timeout->tv_nsec > 1000000000) {
		++abs_timeout->tv_sec;
		abs_timeout->tv_nsec %= 1000000000;
	}
}
