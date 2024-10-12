#include "pch.h"

#include "stopwatch.h"

typedef struct stopwatch
{
	struct timespec startTime;
} stopwatch_t;

stopwatch_t* stopwatch_make(void)
{
	stopwatch_t* sw = walloc_s(sizeof(struct stopwatch));
	memset(&sw->startTime, 0, sizeof(struct timespec));

	return sw;
}

void stopwatch_free(stopwatch_t* const sw)
{
	wfree_s(sw);
}

void stopwatch_start(stopwatch_t* const sw)
{
	clock_gettime(CLOCK_REALTIME, &sw->startTime);
}

unsigned long long int stopwatch_restart(stopwatch_t* const sw)
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	
	unsigned long long elapsed = now.tv_sec - sw->startTime.tv_sec;
	if (now.tv_nsec < sw->startTime.tv_nsec) {
		--elapsed;
		elapsed *= NS_IN_SEC;
		elapsed += sw->startTime.tv_nsec - now.tv_nsec;
	}
	else {
		elapsed *= NS_IN_SEC;
		elapsed += now.tv_nsec - sw->startTime.tv_nsec;
	}

	return elapsed;
}

unsigned long long int stopwatch_lap(stopwatch_t* const sw)
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	
	unsigned long long elapsed = now.tv_sec - sw->startTime.tv_sec;
	if (now.tv_nsec < sw->startTime.tv_nsec) {
		--elapsed;
		elapsed *= NS_IN_SEC;
		elapsed += sw->startTime.tv_nsec - now.tv_nsec;
	}
	else {
		elapsed *= NS_IN_SEC;
		elapsed += now.tv_nsec - sw->startTime.tv_nsec;
	}
	
	sw->startTime = now;

	return elapsed;
}
