/** @file LogLevel.h
* Waola log level enumeration.
*/

#pragma once

namespace Waola {
	/*
	Keep in sync with wlog_level_t
	*/
	enum LogLevel {
		wll_quiet =		-32,
		wll_panic =		-24,
		wll_fatal =		-16,
		wll_error =		-8,
		wll_warning =	0,
		wll_info =		8,
		wll_verbose =	16,
		wll_debug =		24,
		wll_trace =		32
	};
}
