#pragma once

#include <errno.h>
#include <unistd.h>
#include <stdarg.h>

#include <net/if.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#if defined(__APPLE__) || defined(__MACH__)
#include "pch_m.h"
#endif

typedef int BOOL;
#define FALSE 0
#define TRUE 1
