/** @file waolasc_opcode.h
* Waola scanner operation code.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

/**
 * \enum waola scanner operation code.
 * None of the operations provides data.
 */
typedef enum waolasc_opcode {
	wco_undefined,
	wco_idle = 0x01,		/**< Scanner is idle */
	wco_discovering = 0x02,	/**< Scanner is discovering network */
	wco_resolving = 0x04,	/**< Scanner is refreshng state of stored hosts */
	wco_cancelling = 0x08	/**< Scanner is cancelling current operation */
} waolasc_opcode_t;

#if __cplusplus
}   // Extern C
#endif
