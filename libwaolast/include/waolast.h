/** @file waolast.h
* Waola status library.
*
* Provides operations progress.
*/

#pragma once

#include "wmodule.h"
#include "waolast_t.h"

#if __cplusplus
extern "C" {
#endif

const void* wstatus_get(const waolast_t* self,
	wmodule_t* module, int* op_code);
void wstatus_set(waolast_t* self, wmodule_t module,
	int op_code, const void* const restrict data);

#if __cplusplus
}   // Extern C
#endif
