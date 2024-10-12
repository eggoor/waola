/** @file waoladi_opcode.h
* Waola discoverer operation code.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

/**
 * \enum waola discoverer operation code.
 * All operations provide pointer to uint32_t
 * representing IPv4 address in network byte order.
 */
typedef enum waoladi_opcode {
	wdi_undefined,
	wdi_scheduling_arp	= 0x01,
	wdi_sending_arp		= 0x02,
	wdi_got_arp			= 0x04,
	wdi_arp_failed		= 0x08
} waoladi_opcode_t;

#if __cplusplus
}   // Extern C
#endif
