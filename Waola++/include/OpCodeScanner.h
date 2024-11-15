/** @file OpCodeScanner.h
* Waola scanner operation codes.
*/

#pragma once

namespace Waola {
	/**
	* \enum Waola scanner operation code.
	* None of the operations provides data.
	* Keep in sync with waolasc_opcode_t
	*/
	enum OpCodeScanner {
		wco_undefined,
		wco_idle =			0x01,
		wco_discovering =	0x02,
		wco_resolving =		0x04,
		wco_cancelling =	0x08
	};
}
