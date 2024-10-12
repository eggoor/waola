/** @file wdefs.h
* Waola shared library definitions.
*/

#pragma once

/*
Include in precompiled header:

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WS2tcpip.h>
#endif
*/

#if __cplusplus
extern "C" {
#endif

typedef unsigned char byte;

#define WDEF_TRY_COUNT 3 
#define WDEF_WAIT_TIME_MS 256 

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define ETHER_ADDR_LEN (6)
typedef ADDRESS_FAMILY sa_family_t;
#else

typedef int BOOL;
#ifndef	FALSE
#define	FALSE 0
#endif

#ifndef	TRUE
#define	TRUE !FALSE
#endif

#define NO_ERROR (0)
typedef int SOCKET;
#define CALLBACK
#define INFINITE (-1)
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
int closesocket(int s);
#endif

#if __cplusplus
}   // Extern C
#endif
