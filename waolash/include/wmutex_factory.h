/** @file mutex_factory.h
* Waola mutex factory declarations.
*/

#pragma once

#include "wmutex_t.h"

#if __cplusplus
extern "C" {
#endif

typedef wmutex_t* (*wmutex_factory)(void);

wmutex_t* wmake_mutex(void);

#if __cplusplus
}   // Extern C
#endif
