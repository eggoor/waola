#pragma once

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdint.h>
#include <stddef.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "pch_w.h"
#else
#include "pch_u.h"
#endif

#include <wlog.h>
#include <libwaolash.h>
