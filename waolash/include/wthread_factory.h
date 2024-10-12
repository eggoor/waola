#pragma once

#include "wthread_cbi_t.h"
#include "wthread_t.h"

#if __cplusplus
extern "C" {
#endif

typedef wthread_t* (*wthread_factory)(wthread_cbi_t* const);

wthread_t* wmake_thread(wthread_cbi_t* const cbi);

#if __cplusplus
}   // Extern C
#endif
