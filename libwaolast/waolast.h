/** @file waolast.h
* Waola status provider internal interface.
*/

#pragma once

#include "include/waolast.h"

typedef struct waolast waolast_t;

static void wstatus_scanner_set(waolast_t* self,
	waolasc_opcode_t op_code);
static void wstatus_discoverer_set(waolast_t* self,
	int op_code, const void* const restrict data);
static void wstatus_resolver_set(waolast_t* self,
	int op_code, const void* const restrict data);
