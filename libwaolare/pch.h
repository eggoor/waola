#pragma once

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "pch_w.h"
#else
#include "pch_u.h"
#endif

#include <wlog.h>
#include <libwaolash.h>
#include <waolane_sh.h>
#include <waolane.h>
#include <waolast_t.h>
#include <waolast.h>
#include <waolava_sh.h>
#include <waolava.h>
#include <waolath_sh.h>
#include <waolath.h>
#include <waolare_sh.h>
