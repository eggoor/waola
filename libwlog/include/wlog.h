/** @file wlog.h
* Waola logger.
*
* Makes logging to stderr.
*/

#pragma once

/*
Include in precompiled header:

#include <stdarg.h>
*/

#include "wlog_level.h"

#if __cplusplus
extern "C" {
#endif

#define wlog_if_level(level, ...) if (wlog_get_level() >= (level)) \
	wlog(level, __VA_ARGS__)

typedef int BOOL;
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

void wlog_set_level(wlog_level_t level);
BOOL wlog_try_set_level_from_env(void);
wlog_level_t wlog_get_level(void);
void wlog(int level, const char* const fmt, ...);

#if __cplusplus
}   // Extern C
#endif
