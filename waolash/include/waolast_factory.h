/** @file waolast_factory.h
* Waola status library factory.
*
* Provides operations progress.
*/

#pragma once

#include "waolast_t.h"
#include "wmutex_factory.h"

#if __cplusplus
extern "C" {
#endif

	typedef waolast_t* (*waolast_factory)(const wmutex_factory mutex_factory);

	waolast_t* wmake_waolast(const wmutex_factory mutex);
	void wstatus_free(waolast_t* const self);

#if __cplusplus
}   // Extern C
#endif
