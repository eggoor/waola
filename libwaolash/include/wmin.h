/** @file min.h
* Safe C11 MIN macro.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

#ifdef WMIN
#undef WMIN
#endif

#define WMIN(X, Y) (_Generic((X) + (Y),   \
    int:                wmin,             \
    unsigned:           wminu,            \
    long:               wminl,            \
    unsigned long:      wminul,           \
    long long:          wminll,           \
    unsigned long long: wminull,          \
    float:              wminf,            \
    double:             wmind,            \
    long double:        wminld)((X), (Y)))

inline int wmin(int const x, int const y)
{
    return y < x ? y : x;
}

inline unsigned wminu(unsigned const x, unsigned const y)
{
    return y < x ? y : x;
}

inline long wminl(long const x, long const y)
{
    return y < x ? y : x;
}

inline unsigned long wminul(unsigned long const x, unsigned long const y)
{
    return y < x ? y : x;
}

inline long long wminll(long long const x, long long const y)
{
    return y < x ? y : x;
}

inline unsigned long long wminull(unsigned long long const x, unsigned long long const y)
{
    return y < x ? y : x;
}

inline float wminf(float const x, float const y)
{
    return y < x ? y : x;
}

inline double wmind(double const x, double const y)
{
    return y < x ? y : x;
}

inline long double wminld(long double const x, long double const y)
{
    return y < x ? y : x;
}

#if __cplusplus
}   // Extern C
#endif
