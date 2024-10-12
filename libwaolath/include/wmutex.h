/** @file wthread.h
* Waola mutex.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

int wmutex_do_make_impl(void** const restrict ppm);
void wmutex_do_free_impl(void* const restrict mutex);

BOOL wmutex_lock_impl(const void* const restrict mutex, int timeout_ms);
BOOL wmutex_try_lock_once_impl(void* const restrict mutex);
void wmutex_release_impl(void* const restrict mutex);

#if __cplusplus
}   // Extern C
#endif
