#pragma once

#include "wthread_counter_t.h"

#if __cplusplus
extern "C" {
#endif

typedef wthread_counter_t* (*wthread_counter_factory)(const wmutex_factory);

wthread_counter_t* wmake_thread_counter(const wmutex_factory mutex_factory);
void wthread_counter_free(wthread_counter_t* const tc);

#if __cplusplus
}   // Extern C
#endif

