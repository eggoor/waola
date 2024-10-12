/** @file waolasc_opcode.h
* Waola discoverer operation code.
*/

#pragma once

namespace WaolaCli {
	/**
	* \enum Waola scanner operation code.
	* None of the operations provides data.
	* Keep in sync with waolasc_opcode_t
	*/
	public enum class OpCodeScanner {
		Undefined,
		Idle = 0x01,
		Discovering = 0x02,
		Resolving = 0x04,
		Cancelling = 0x08
	};
}