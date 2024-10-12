/** @file event_factory.h
* Waola event factory declarations.
*/

#pragma once

#include "wevent_t.h"

#if __cplusplus
extern "C" {
#endif

typedef wevent_t* (*wevent_factory)(void);

wevent_t* wmake_event(void);

#if __cplusplus
}   // Extern C
#endif

