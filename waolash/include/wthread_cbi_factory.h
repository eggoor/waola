#pragma once

#include "wthread_cbi_t.h"

#if __cplusplus
extern "C" {
#endif

typedef wthread_cbi_t* (*wthread_cbi_factory)(void*, wthreadCb,
	wthreadFinishedCb, wthreadFinishedCb, const void*);

wthread_cbi_t* wmake_thread_cbi(void* thread_owner,
	wthreadCb thread_cb, wthreadFinishedCb finished_cb,
	wthreadFinishedCb child_finished_cb, const void* const extra_data);

#if __cplusplus
}   // Extern C
#endif

