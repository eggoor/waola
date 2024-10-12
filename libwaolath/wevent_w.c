#include "pch.h"

#include "wevent.h"

int wevent_do_make_impl(void** const restrict ppe)
{
	int rc = 0;

	if (!ppe) {
		wlog_if_level(wll_error, "No event pointer provided\n");
		return - 1;
	}

	HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (event) {
		*ppe = walloc_s(sizeof(event));
		memcpy(*ppe, &event, sizeof(event));
	}
	else {
		*ppe = NULL;
		rc = GetLastError();
		wlog_if_level(wll_error, "CreateEvent failed: %d\n", rc);
	}

	return 0;
}

void wevent_do_free_impl(void* const restrict e)
{
	CloseHandle(*(HANDLE*)e);
	wfree_s(e);
}

BOOL wevent_wait_impl(void* const restrict e, int timeout_ms)
{
	DWORD dwResult = WaitForSingleObject(*(HANDLE*)e, timeout_ms);

	if (WAIT_FAILED == dwResult || WAIT_ABANDONED == dwResult) {
		DWORD dwError = GetLastError();
		wlog_if_level(wll_error, "WaitForSingleObject(m) failed: result == 0x%p, error == %d\n",
			dwResult, dwError);
	}

	return WAIT_OBJECT_0 == dwResult;
}

void wevent_set_impl(void* const restrict e)
{
	if (!SetEvent(*(HANDLE*)e)) {
		wlog_if_level(wll_error, "SetEvent failed: %d\n",
			GetLastError());
	}
}
