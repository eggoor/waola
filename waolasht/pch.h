#pragma once

#if HAVE_CONFIG_H
# include <config.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "pch_w.h"
#else
#include "pch_u.h"
#endif

#include <stdint.h>
#include <assert.h>
#include <time.h>

#include <wlog.h>
#include <waolash.h>
#include <libwaolash.h>
int wsockaddr_get_len_s(sa_family_t sa_family);

#define IP_ADDRESS(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | d)
