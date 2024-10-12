/** @file wevent.h
* Waola event.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef struct wevent wevent_t;

void wevent_free(wevent_t* self);

void wevent_wait(wevent_t* self, int timeout_ms);
void wevent_set(wevent_t* self);

#if __cplusplus
}   // Extern C
#endif
