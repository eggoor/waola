#include "pch.h"

#include "include/wmutex.h"

int wmutex_do_make_impl(void** ppm)
{
	int rc = 0;

	if (!ppm) {
		wlog_if_level(wll_error, "No mutex pointer provided\n");
		return - 1;
	}

	HANDLE mutex = CreateMutex(NULL, FALSE, NULL);

	if (mutex) {
		*ppm = walloc_s(sizeof(mutex));
		memcpy(*ppm, &mutex, sizeof(mutex));
	}
	else {
		*ppm = NULL;
		rc = GetLastError();
		wlog_if_level(wll_error, "CreateMutex failed with %d\n",
			rc);
	}

	return rc;
}

void wmutex_do_free_impl(void* const restrict m)
{
	CloseHandle(*(HANDLE*)m);
	wfree_s(m);
}

BOOL wmutex_lock_impl(const void* const restrict m, int timeout)
{
	DWORD dwResult = WaitForSingleObject(*(HANDLE*)m, timeout);

	if (WAIT_FAILED == dwResult || WAIT_ABANDONED == dwResult) {
		DWORD dwError = GetLastError();
		wlog_if_level(wll_error, "WaitForSingleObject(m) failed: result == 0x%p, error == %d\n",
			dwResult, dwError);
	}

	return WAIT_OBJECT_0 == dwResult;
}

BOOL wmutex_try_lock_once_impl(void* const restrict m)
{
	return WaitForSingleObject(*(HANDLE*)m, 0) == WAIT_OBJECT_0;
}

void wmutex_release_impl(void* const restrict m)
{
	if (!ReleaseMutex(*(HANDLE*)m)) {
		wlog_if_level(wll_error, "ReleaseMutex failed\n");
	}
}
