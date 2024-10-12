/** @file wlog_level.h
* Waola log levels definition.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef enum wlog_level {
	wll_quiet = -32,
	wll_panic = -24,
	wll_fatal = -16,
	wll_error = -8,
	wll_warning = 0,
	wll_info	= 8,
	wll_verbose	= 16,
	wll_debug	= 24,
	wll_trace	= 32
} wlog_level_t;

#if __cplusplus
}   // Extern C
#endif
