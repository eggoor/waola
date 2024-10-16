/** @file wmax.h
* Safe C11 MAX macro.
*/

#pragma once

#if __cplusplus
extern "C" {
#endif

#ifdef WMAX
#undef WMAX
#endif

#define WMAX(X, Y) (_Generic((X) + (Y),   \
    int:                wmax,             \
    unsigned:           wmaxu,            \
    long:               wmaxl,            \
    unsigned long:      wmaxul,           \
    long long:          wmaxll,           \
    unsigned long long: wmaxull,          \
    float:              wmaxf,            \
    double:             wmaxd,            \
    long double:        wmaxld)((X), (Y)))

inline int wmax(int const x, int const y)
{
    return y > x ? y : x;
}

inline unsigned wmaxu(unsigned const x, unsigned const y)
{
    return y > x ? y : x;
}

inline long wmaxl(long const x, long const y)
{
    return y > x ? y : x;
}

inline unsigned long wmaxul(unsigned long const x, unsigned long const y)
{
    return y > x ? y : x;
}

inline long long wmaxll(long long const x, long long const y)
{
    return y > x ? y : x;
}

inline unsigned long long wmaxull(unsigned long long const x, unsigned long long const y)
{
    return y > x ? y : x;
}

inline float wmaxf(float const x, float const y)
{
    return y > x ? y : x;
}

inline double wmaxd(double const x, double const y)
{
    return y > x ? y : x;
}

inline long double wmaxld(long double const x, long double const y)
{
    return y > x ? y : x;
}

#if __cplusplus
}   // Extern C
#endif
