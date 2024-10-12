/** @file eWindowPos.h
* Library to inscribe window in screen for dynamic linking.
*/

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "eWindowPosApiW.h"
#else
#include "eWindowPosApiU.h"
#endif

#include "eWindowPosPrimitives.h"

/**
 * Inscribes window rectangle in screen(s).
 *
 * @param rc_wnd window rectangle.
 * @param screens screen rectanle(s).
 * @param screen_count count of @screens.
 * @param adjusted Output parameter. Function sets it
*  to TRUE if window rectange was modified.
 *
 * @return TRUE if window has been successfully inscribed.
 */
EWINDOWPOS_API BOOL adjust_wnd_rect(rect_t* const rc_wnd,
	const rect_t* const* const screens,
	const int screen_count,
	BOOL* const adjusted);
