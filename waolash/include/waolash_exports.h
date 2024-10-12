/** @file waolash_exports.h
* WAOLASHD_API definition.
*/

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "waolash_exports_w.h"
#else
#include "waolash_exports_u.h"
#endif
