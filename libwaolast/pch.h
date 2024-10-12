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
#include <waolath.h>
#include <waoladi_opcode.h>
#include <waolare_opcode.h>
#include <waolasc_opcode.h>
