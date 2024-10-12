#include "pch.h"

#include "include/waolash_exports.h"

WAOLASHD_API void waolash_set_log_level(wlog_level_t level)
{
	wlog_set_level(level);
}
