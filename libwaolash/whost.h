#pragma once

#include "include/whost_s.h"

BOOL whost_set_display_name(whost_t* host,
	const char* const restrict displayName);
BOOL whost_set_op_result(whost_t* const restrict host,
	const whost_op_result_t opr);
