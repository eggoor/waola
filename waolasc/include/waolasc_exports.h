/** @file waolasc_exports.h
* WAOLACOD_API definition.
*/

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "waolasc_exports_w.h"
#else
#include "waolasc_exports_u.h"
#endif
