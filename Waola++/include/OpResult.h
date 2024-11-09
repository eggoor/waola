/** @file OpResult.h
* Waola wakeip result enumeration.
*/

#pragma once

namespace Waola {
	/*
	Keep in sync with whost_op_result_t
	*/
	enum OpResult {
		wor_undefined
		, wor_success
		, wor_fail
	};
}
