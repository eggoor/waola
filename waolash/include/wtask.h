/** @file wtask.h
* Waola scanner's task flags.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

/**
 * \enum Waola scanner 's task flags.
 */
typedef enum wtask {
	wt_idle
	, wt_discover = 0x01	/**< Scanner is discovering network */
	, wt_refresh = 0x02		/**< Scanner is refreshing known hosts */
} wtask_t;

#if __cplusplus
}   // Extern C
#endif
