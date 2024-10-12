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
#include <waolash_t.h>
#include <libwaolash.h>
#include <waolath_sh.h>
#include <waolath.h>
#include <waolane_sh.h>
#include <waolane.h>
#include <../libwaolane/waolane_fact4s.h>
