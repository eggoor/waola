/** @file waolava_opcode.h
* Waola vault operation codes.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef enum waolava_op {
	wva_undefined
	, wva_added		/**< New host is added to the vault */
	, wva_modified	/**< Some property of the stored host is changed */
	, wva_deleted	/**< A host is deleted from the vault */
} waolava_op_t;

#if __cplusplus
}   // Extern C
#endif
