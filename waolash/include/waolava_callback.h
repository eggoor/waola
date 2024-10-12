/** @file waolava_callback.h
* Waola vault callback declarations.
*/

#pragma once

#include "waolava_opcode.h"

#if __cplusplus
extern "C" {
#endif

/**
 * \struct Waola vault callback data.
 */
typedef struct waolava_cbi {
	const void* caller;
	whost_view_t* hostView;
	const waolava_op_t opCode;
} waolava_cbi_t;

/**
 * Callback shich is called each time
 * when some vault operation is finished.
 *
 * @param cbi information about vault operation.
 */
typedef void (*waolava_callback)(const waolava_cbi_t* const cbi);

typedef struct waolava_subscribe_data {
	const void* caller;
	waolava_callback cb;
} waolava_subscribe_data_t;

#if __cplusplus
}   // Extern C
#endif
