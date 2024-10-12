/** @file waolash.h
* Waola shared library for dynamic linking.
*/

#pragma once
#include <time.h>
#include <stdint.h>

#include "waolash_t.h"
#include "wmem.h"
#include "wstr.h"
#include "waolash_fact4s_sh.h"
#include "wcont4r.h"
#include "wsockaddr.h"
#include "whost_data.h"
#include "whost.h"
#include "whost_view.h"

#if __cplusplus
extern "C" {
#endif

WAOLASHD_API void waolash_set_log_level(wlog_level_t level);

#if __cplusplus
}   // Extern C
#endif
