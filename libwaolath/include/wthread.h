/** @file wthread.h
* Waola thread.
*/

#pragma once

#include <wthread_t.h>

#if __cplusplus
extern "C" {
#endif

int wthread_join(wthread_t* const restrict t);
int wthread_free(wthread_t* const restrict t, int timeout_ms);
void wthread_sleep(int32_t ms);

#if __cplusplus
}   // Extern C
#endif
