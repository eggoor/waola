/** @file libwaolash.h
* Waola shared library.
*/

#pragma once

#include <waolash_t.h>

#include "wmin.h"
#include "wmax.h"
#include "wmem_s.h"
#include "wstr_s.h"
#include "wlist_s.h"
#include "wcont4r_s.h"
#include "wmac_addr_s.h"
#include "wsockaddr_s.h"
#include "whost_data_s.h"
#include "whost_s.h"
#include "whost_view_s.h"
#include "wasync_callback.h"

#if __cplusplus
extern "C" {
#endif

int waola_prepare(void);
int waola_finish(void);

int calc_set_bits(uint32_t n);

typedef void (*wcanceller)(void* const restrict p);

int get_system_error_s(void);
int get_socket_error_s(void);

#ifndef ARRSIZE
#define ARRSIZE(a) ((sizeof(a))/(sizeof(*a)))
#endif

#if __cplusplus
}   // Extern C
#endif
