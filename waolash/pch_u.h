#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

#if defined (__linux__) || defined(__linux) || defined(linux)
#include "pch_l.h"
#endif

typedef int BOOL;
#define FALSE 0
#define TRUE 1
