#include "pch.h"

#include "wlog_level_textual.h"

#ifndef ARRSIZE
#define ARRSIZE(a) ((sizeof(a))/(sizeof(*a)))
#endif

typedef struct log_level_pair {
	const char* name;
	wlog_level_t level;
} log_level_pair_t;

static log_level_pair_t log_levels[] = {
	{"quiet", wll_quiet}
	, {"panic", wll_panic}
	, {"fatal", wll_fatal}
	, {"error", wll_error}
	, {"warning", wll_warning}
	, {"info", wll_info}
	, {"verbose", wll_verbose}
	, {"debug", wll_debug}
	, {"trace", wll_trace}
};

wlog_level_t wlog_level_from_string(char* const restrict log_level_str)
{
	wlog_level_t ll = wll_warning;
	
	if (log_level_str) {
		errno = 0;
		char *endptr = NULL;
		long val = strtol(log_level_str, &endptr, 0);
		if (log_level_str == endptr
			|| (0 != errno && 0 == val)) {
			char* lower = log_level_str;
			do {
				*lower = tolower(*lower);
			}
			while (*++lower);
			
			for (size_t i = ARRSIZE(log_levels); i --> 0;) {
				if (strcmp(log_level_str, log_levels[i].name) == 0) {
					ll = log_levels[i].level;
					break;
				}
			}
		}
	}
	
	return ll;
}
