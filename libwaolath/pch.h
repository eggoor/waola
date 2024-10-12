#pragma once

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "pch_w.h"
#else
#include "pch_u.h"
#endif

#include <wlog.h>
#include <libwaolash.h>
#include <waolath_sh.h>

void timeout_to_abs_timeout(int ms, struct timespec* const restrict abs_timeout);
