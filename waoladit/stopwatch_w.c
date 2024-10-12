#include "pch.h"

#include "stopwatch.h"

typedef struct stopwatch
{
	FILETIME startTime;
} stopwatch_t;

stopwatch_t* stopwatch_make()
{
	return walloc_s(sizeof(struct stopwatch));
}

void stopwatch_free(stopwatch_t* sw)
{
	wfree_s(sw);
}

void stopwatch_start(stopwatch_t* sw)
{
	GetSystemTimeAsFileTime(&sw->startTime);
}

unsigned long long int stopwatch_restart(stopwatch_t* sw)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);

	ULARGE_INTEGER uliStart = {
		.LowPart = sw->startTime.dwLowDateTime,
		.HighPart = sw->startTime.dwHighDateTime
	};

	ULARGE_INTEGER uliStop = {
		.LowPart = now.dwLowDateTime,
		.HighPart = now.dwHighDateTime
	};

	sw->startTime = now;
	
	return (uliStop.QuadPart - uliStart.QuadPart) * 100;
}

unsigned long long int stopwatch_lap(stopwatch_t* sw)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);

	ULARGE_INTEGER uliStart = {
		.LowPart = sw->startTime.dwLowDateTime,
		.HighPart = sw->startTime.dwHighDateTime
	};

	ULARGE_INTEGER uliStop = {
		.LowPart = now.dwLowDateTime,
		.HighPart = now.dwHighDateTime
	};

	return (uliStop.QuadPart - uliStart.QuadPart) * 100;
}
