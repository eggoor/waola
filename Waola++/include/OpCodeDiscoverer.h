/** @file OpCodeDiscoverer.h
* Waola discoverer operation code.
*/

#pragma once

namespace Waola {
	/**
	* \enum Waola discoverer operation code.
	* All operations provide pointer to uint32_t
	* representing IPv4 address in network byte order.
	* Keep in sync with waoladi_opcode_t
	*/
	enum OpCodeDiscoverer {
		wdi_undefined,
		wdi_scheduling_arp =	0x01,	/**< Provides pointer to uint32_t */
		wdi_sending_arp =		0x02,	/**< Provides pointer to uint32_t */
		wdi_got_arp =			0x04,	/**< Provides pointer to uint32_t */
		wdi_arp_failed =		0x08	/**< Provides pointer to uint32_t */
	};
}
