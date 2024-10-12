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
#include <waolash.h>
#include <libwaolash.h>
#include <waolath_sh.h>
#include <waolane_sh.h>
#include <libwaola.h>
