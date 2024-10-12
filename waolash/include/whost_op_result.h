/** @file whost_op_result.h
* Waola wakeup result definition.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef enum whost_op_result
{
	wor_undefined
	, wor_success
	, wor_fail
} whost_op_result_t;

#if __cplusplus
}   // Extern C
#endif
