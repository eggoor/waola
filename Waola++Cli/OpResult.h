/** @file OpResult.h
* Waola wakeup result.
*/

#pragma once

namespace WaolaCli {
	/*
	Keep in sync with Waola::OpResult
	*/
	public enum class OpResult {
		Undefined
		, Success
		, Fail
	};
}