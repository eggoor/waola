#include "pch.h"

#include "wthread.h"

void wthread_do_make_impl(void** const restrict ppt, wthread_cbi_t* cbi)
{
	if (!ppt) {
		wlog_if_level(wll_error, "No thread pointer provided\n");
		return;
	}

	HANDLE t = CreateThread(
		NULL,					// default security attributes
		0,						// use default stack size	
		wthread_thread_func,	// thread function name
		cbi,					// argument to thread function 
		0,						// use default creation flags 
		NULL);					// returns the thread identifier

	if (t) {
		*ppt = walloc_s(sizeof(t));
		memcpy(*ppt, &t, sizeof(t));
	}
	else {
		*ppt = NULL;
		int error = get_system_error_s();
		wlog_if_level(ERROR_NOT_ENOUGH_MEMORY == error ? wll_debug : wll_error,
			"CreateThread failed: %d\n", error);
	}
}

int wthread_do_free_impl(void* t, int timeout_ms)
{
	int rc = 0;

	if (timeout_ms != 0) {
		rc = wthread_do_join_impl(t);
	}

	CloseHandle(*(HANDLE*)t);
	wfree_s(t);

	return rc;
}

void wthread_sleep(int32_t ms)
{
	Sleep(ms);
}

int CALLBACK wthread_thread_func(void* const restrict cbd)
{
	wthread_cbi_t* cbi = cbd;

	int rc = (*cbi->threadCb)(cbi);

	if (rc) {
		cbi->errNo = rc;
	}
	else if (0 != errno && ETIMEDOUT != errno) {
		cbi->errNo = errno;
	}

	if (cbi->threadFinishedCb) {
		(*cbi->threadFinishedCb)(cbi);
	}

	return errno;
}

int wthread_do_join_impl(void* const restrict t)
{
	int waitResult = WaitForSingleObject(*(HANDLE*)t, INFINITE);

	if (WAIT_OBJECT_0 != waitResult) {
		wlog_if_level(wll_warning, "WaitForSingleObject(%d) failed: %d\n",
			INFINITE, waitResult);
	}

	return waitResult;
}