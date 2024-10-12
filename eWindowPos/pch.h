#pragma once

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdint.h>
#include <limits.h>
#include <math.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "pch_w.h"
#else
#include "pch_u.h"
#endif

void* ealloc(size_t bc);
void efree(void* p);
