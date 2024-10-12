/** @file wmodule.h
* Waola modules enumeration.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

typedef enum wmodule {
	wm_undefined
	, wm_scanner
	, wm_discoverer
	, wm_resolver
} wmodule_t;

#if __cplusplus
}   // Extern C
#endif
