/** @file Task.h
* Waola scanner rask enumeration.
*/

#pragma once

namespace Waola {
	/*
	Keep in sync with wtask_t
	*/
	enum Task {
		wt_idle
		, wt_discover =	0x01
		, wt_refresh =	0x02
	};
}
