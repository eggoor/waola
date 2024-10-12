#pragma once

#if __cplusplus
extern "C" {
#endif

int strcpy_s(char* restrict dst, size_t dstsz, const char* restrict src);
int strncpy_s(char* const restrict dst, size_t dstsz, const char* const restrict src, size_t len);
int strcat_s(char* restrict dst, size_t dstsz, const char* restrict src);
int sprintf_s(char* const restrict buffer, size_t dstsz, const char* const format, ...);

#if __cplusplus
}   // Extern C
#endif
