#pragma once

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <forward_list>
#include <functional>
#include <algorithm>
#include <sstream>
#include <future>

#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "pch_w.h"
#else
#include "pch_u.h"
#endif

#include <wlog.h>
#include <wstr.h>
#include <whost_data.h>
#include <whost_view.h>
#include <wcont4r.h>
#include <waolasc.h>
#include <waola.h>

char* StrDup(const char* const src);
