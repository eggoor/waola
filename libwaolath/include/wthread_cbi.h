#pragma once

#include <wthread_cbi_t.h>

#if __cplusplus
extern "C" {
#endif

typedef struct wthread_cbi {
#if defined(DEBUG) || defined(_DEBUG)
	char ownerId[THR_CBI_ID_BUF_SIZE];
#endif
	void* threadOwner;
	wthreadCb threadCb;
	wthreadFinishedCb threadFinishedCb;
	wthreadFinishedCb childThreadFinishedCb;
	const void* extraData;
	int errNo;
} wthread_cbi_t;

void wthread_cbi_free(wthread_cbi_t* const restrict cbi);

#if __cplusplus
}   // Extern C
#endif

