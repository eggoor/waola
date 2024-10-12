#include "pch.h"

int calc_set_bits(uint32_t n)
{
    n = n - ((n >> 1) & 0x55555555);                // add pairs of bits
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333); // quads
    n = (n + (n >> 4)) & 0x0F0F0F0F;                // groups of 8
    n *= 0x01010101;                                // horizontal sum of bytes
    // return just that top byte (after truncating to 32-bit even when int is wider than uint32_t)
    return  n >> 24;
}