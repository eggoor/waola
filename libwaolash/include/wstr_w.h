#pragma once

#if __cplusplus
extern "C" {
#endif

char* wstr_16_to_8_s(const wchar_t* const restrict wstr);
wchar_t* wstr_8_to_16_s(const char* const restrict str);

#if __cplusplus
}   // Extern C
#endif
