/** @file waolash_fact4s.h
* Factories for Waola shared library.
*/

#pragma once

#include "wcont4r_factory.h"
#include "whost_factory.h"
#include "whost_view_factory.h"
#include "waolash_exports.h"

#if __cplusplus
extern "C" {
#endif

WAOLASHD_API wcont4r_t* wmake_cont4r(wcont_type_t ct);

#if __cplusplus
}   // Extern C
#endif
