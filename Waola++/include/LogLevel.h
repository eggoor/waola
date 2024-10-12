#pragma once

namespace Waola {
	/*
	Keep in sync with wlog_level_t
	*/
	enum LogLevel {
		wll_quiet = -8,
		wll_panic = 0,
		wll_fatal = 8,
		wll_error = 16,
		wll_warning = 24,
		wll_info = 32,
		wll_verbose = 40,
		wll_debug = 48,
		wll_trace = 56
	};
}
