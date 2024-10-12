/** @file wthread_counter.h
* Waola thread counter.
*/

#pragma once

#include <wthread_counter_t.h>

#if __cplusplus
extern "C" {
#endif

BOOL wthread_counter_increase_thread_count(wthread_counter_t* const restrict tc);
void wthread_counter_decrease_thread_count(wthread_counter_t* const restrict tc);
uint32_t wthread_counter_get_thread_count(wthread_counter_t* const restrict tc);
void wthread_counter_thread_finished_callback(void* const restrict threadOwner);
void wthread_counter_set_max_threads(wthread_counter_t* const restrict tc, uint32_t max_threads);

#if __cplusplus
}   // Extern C
#endif

