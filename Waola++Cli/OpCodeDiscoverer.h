/** @file waoladi_opcode.h
* Waola discoverer operation code.
*/

#pragma once

namespace WaolaCli {
	/**
	* \enum waola discoverer operation code.
	* All operations provide pointer to uint32_t
	* representing IPv4 address.
	* Keep in sync with waoladi_opcode_t
	*/
	public enum class OpCodeDiscoverer {
		Undefined,
		SchedulingArp = 0x01,	/**< Provides pointer to uint32_t */
		SendingArp = 0x02,		/**< Provides pointer to uint32_t */
		GotArp = 0x04,			/**< Provides pointer to uint32_t */
		ArpFailed = 0x08		/**< Provides pointer to uint32_t */
	};
}