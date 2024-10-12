/** @file waolash_exports_w.h
* WAOLASHD_API for Windows.
*/#pragma once

#if __cplusplus
extern "C" {
#endif

#ifdef WAOLASHD_EXPORTS
#define WAOLASHD_API __declspec( dllexport )
#else
#define WAOLASHD_API __declspec( dllimport )
#endif

#if __cplusplus
}   // Extern C
#endif
