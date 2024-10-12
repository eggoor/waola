/** @file wmutex_t.h
* Forward declaration of Waola mutex.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

#define DEF_LOCK_TIMEOUT_MS 256

typedef struct wmutex wmutex_t;

void wmutex_free(wmutex_t* const mutex);

BOOL wmutex_lock(const wmutex_t* const mutex, int timeout_ms);
BOOL wmutex_try_lock(const wmutex_t* const mutex);
BOOL wmutex_try_lock_once(const wmutex_t* const mutex);
void wmutex_release(wmutex_t* const mutex);

#if __cplusplus
}   // Extern C
#endif
