/** @file waolasc_callback.h
* Waola scanner callback definition.
*/

#pragma once

#include "wtask.h"

#if __cplusplus
extern "C" {
#endif

/**
 * \struct waola scanner state callback data.
 */
typedef struct waolasc_cbi
{
	const void* caller;
	const wtask_t tasks;
} waolasc_cbi_t;

/**
 * Callback called each time
 * when scanner state is changed.
 *
 * @param cbi informatin about currently running tasks.
 */
typedef void (*waolasc_callback)(const waolasc_cbi_t* const cbi);

typedef struct waolasc_subscribe_data
{
	const void* caller;
	waolasc_callback cb;
} waolasc_subscribe_data_t;

#if __cplusplus
}   // Extern C
#endif
