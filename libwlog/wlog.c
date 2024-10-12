#include "pch.h"

#include "include/wlog.h"
#include "wlog_level_textual.h"

static const char* env_var_name = "WAOLA_LOG_LEVEL";

static wlog_level_t log_level = wll_warning;

static void vlog(const char* const restrict fmt, va_list argp);

void wlog(wlog_level_t level, const char* const fmt, ...)
{
	if (level > log_level) {
		return;
	}

	va_list argp;
	va_start(argp, fmt);
	vlog(fmt, argp);
	va_end(argp);
}

void wlog_set_level(wlog_level_t level)
{
	log_level = level;
}

wlog_level_t wlog_get_level(void)
{
	return log_level;
}

static void vlog(const char* const restrict fmt, va_list argp)
{
	vfprintf(stderr, fmt, argp);
}

BOOL wlog_try_set_level_from_env(void)
{
	BOOL level_changed = FALSE;
	char* log_level_str = getenv(env_var_name);

	if (log_level_str) {
		const wlog_level_t level_from_env = wlog_level_from_string(log_level_str);
		if (level_from_env != log_level) {
			log_level = level_from_env;
			level_changed = TRUE;
		}
	}
	
	return level_changed;
}
