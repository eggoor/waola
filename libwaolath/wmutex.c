#include "pch.h"

#include "include/wmutex.h"

struct wmutex
{
	void* mimpl;
};

wmutex_t* wmutex_make_impl(void)
{
	wmutex_t* mutex = walloc_s(sizeof(struct wmutex));

	if (!mutex) {
		wlog_if_level(wll_warning, "No %zuB of free memory for wmutex\n",
			sizeof(struct wmutex));
		goto end;
	}

	int rc = wmutex_do_make_impl(&mutex->mimpl);
	if (rc) {
		wfree_s(mutex);
		mutex = NULL;
	}

end:
	return mutex;
}

void wmutex_free(wmutex_t* const restrict mutex)
{
	wmutex_do_free_impl(mutex->mimpl);
	wfree_s(mutex);
}

BOOL wmutex_try_lock(const wmutex_t* const restrict mutex)
{
	BOOL lckd = FALSE;

	for (int i = 0; !lckd && i < WDEF_TRY_COUNT; ++i) {
		lckd = wmutex_lock(mutex,
			DEF_LOCK_TIMEOUT_MS << i);
	}

	if (!lckd) {
		wlog_if_level(wll_verbose, "Couldn't lock mutex\n");
	}

	return lckd;
}

BOOL wmutex_try_lock_once(const wmutex_t* const restrict mutex)
{
	return wmutex_try_lock_once_impl(mutex->mimpl);
}

BOOL wmutex_lock(const wmutex_t* const restrict mutex, int ms)
{
	return wmutex_lock_impl(mutex->mimpl, ms);
}

void wmutex_release(wmutex_t* const restrict mutex)
{
	wmutex_release_impl(mutex->mimpl);
}
