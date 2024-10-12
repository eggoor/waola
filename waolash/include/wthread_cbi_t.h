/** @file wthread_cbi_t.h
* Forward declaration of Waola thread callback info.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

	typedef struct wthread_cbi wthread_cbi_t;
	typedef int (*wthreadCb)(const wthread_cbi_t* const);
	typedef void (*wthreadFinishedCb)(const wthread_cbi_t* const);

#if defined(DEBUG) || defined(_DEBUG)
#define THR_CBI_ID_BUF_SIZE 32
#define THR_CBI_ID_MAX_LEN (THR_CBI_ID_BUF_SIZE - 1)
#endif

#if __cplusplus
}   // Extern C
#endif
