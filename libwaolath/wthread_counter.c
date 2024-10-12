#include "pch.h"

#include "include/wthread.h"
#include "include/wmutex.h"
#include "wthread_counter_max.h"

#include "include/wthread_counter.h"

#define THREAD_COUNTER_TRY_COUNT 4
#define THREAD_COUNTER_TIMEOUT 512
#define MIN_THREAD_COUNT 2

struct wthread_counter
{
	uint32_t maxThreadCount;
	uint32_t threadCount;
	wmutex_t* mutex;
};

static void wthread_counter_lock(const wthread_counter_t* const restrict tc);
static void wthread_counter_release(const wthread_counter_t* const restrict tc);

wthread_counter_t* wthread_counter_make_impl(const wmutex_factory mutex_factory)
{
	wthread_counter_t* tc = walloc_s(sizeof(struct wthread_counter));

	if (!tc) {
		wlog_if_level(wll_warning, "No %zuB of free memory for thread counter\n",
			sizeof(struct wthread_counter));
		goto end;
	}

	tc->mutex = (*mutex_factory)();

	if (!tc->mutex) {
		wfree_s(tc);
		tc = NULL;
		wlog_if_level(wll_warning, "Unable to make thread counter mutex\n");
		goto end;
	}

	tc->maxThreadCount = calc_max_thread_count();
	tc->threadCount = 0;

end:
	return tc;
}

void wthread_counter_free(wthread_counter_t* const restrict tc)
{
	wmutex_free(tc->mutex);
	wfree_s(tc);
}

void wthread_counter_set_max_threads(wthread_counter_t* const restrict tc, uint32_t max_threads)
{
	if (max_threads < MIN_THREAD_COUNT) {
		wlog_if_level(wll_warning, "%s: will set default minimum value %d instead of provided %d\n",
			__func__, MIN_THREAD_COUNT, max_threads);
		max_threads = MIN_THREAD_COUNT;
	}
	tc->maxThreadCount = max_threads;
}

BOOL wthread_counter_increase_thread_count(wthread_counter_t* const restrict tc)
{
	BOOL increased = FALSE;
	
	for (int i = 0; i < THREAD_COUNTER_TRY_COUNT; ++i) {
		wthread_counter_lock(tc);

		if (tc->threadCount < tc->maxThreadCount) {
			++tc->threadCount;
			wlog_if_level(wll_trace, "Th. count: %d>%d \n",
				tc->threadCount - 1, tc->threadCount);
			wthread_counter_release(tc);
			increased = TRUE;
			break;
		}

		wthread_counter_release(tc);

		wlog_if_level(wll_debug, "%d thread count reached, gonna sleep for %d ms\n",
			tc->threadCount, THREAD_COUNTER_TIMEOUT << i);
		wthread_sleep(THREAD_COUNTER_TIMEOUT << i);
	}
	
	if (!increased) {
		wlog_if_level(wll_warning, "Unable to increase thread counter after %d tries\n",
			THREAD_COUNTER_TRY_COUNT);
	}
	
	return increased;
}

void wthread_counter_decrease_thread_count(wthread_counter_t* const restrict tc)
{
	wthread_counter_lock(tc);

	if (tc->threadCount > 0) {
		--tc->threadCount;
		wlog_if_level(wll_trace, "Th. count: %d<%d \n",
			tc->threadCount, tc->threadCount + 1);
	}
	else {
		wlog_if_level(wll_warning, "There are more threads finished then started\n");
	}

	wthread_counter_release(tc);
}

uint32_t wthread_counter_get_thread_count(wthread_counter_t* const restrict tc)
{
	uint32_t threadCount;
	wthread_counter_lock(tc);	
	threadCount = tc->threadCount;
	wmutex_release(tc->mutex);

	return threadCount;
}

void wthread_counter_thread_finished_callback(void* const restrict threadOwner)
{
	wthread_counter_t* tc = threadOwner;
	wthread_counter_decrease_thread_count(tc);
}

static void wthread_counter_lock(const wthread_counter_t* const restrict tc)
{
	for (int i = 0; !wmutex_try_lock(tc->mutex); ++i) {
		wthread_sleep(THREAD_COUNTER_TIMEOUT << i);
	}
}

static void wthread_counter_release(const wthread_counter_t* const restrict tc)
{
	wmutex_release(tc->mutex);
}
