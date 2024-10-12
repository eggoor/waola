#pragma once

#include "include/wthread_cbi.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "wthread_w.h"
#else
#include "wthread_u.h"
#endif

void wthread_do_make_impl(void** const restrict ppt, wthread_cbi_t* const restrict cbi);
int wthread_do_join_impl(void* const restrict t);
int wthread_do_free_impl(void* const restrict t, int timeout_ms);
