#pragma once

#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sysexits.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <net/ethernet.h>
#include <arpa/inet.h>

#include <netinet/if_ether.h>

#if defined(__APPLE__) || defined(__MACH__)
#include "pch_m.h"
#elif defined (__linux__) || defined(__linux) || defined(linux)
#include "pch_l.h"
#endif

typedef int BOOL;
#define FALSE 0
#define TRUE 1
