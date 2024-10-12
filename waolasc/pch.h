#pragma once

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdint.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "pch_w.h"
#else
#include "pch_u.h"
#endif

#include <wlog.h>
#include <libwaolash.h>
#include <waolath_sh.h>
#include <waolava_sh.h>
#include <waolane_sh.h>
#include <waolast_sh.h>
#include <waoladi_sh.h>
#include <waolare_sh.h>
#include <waolasc_sh.h>
#include <libwaolasc.h>
