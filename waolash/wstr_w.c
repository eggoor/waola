
#include "pch.h"

#include "include/waolash_exports.h"

WAOLASHD_API char* wstr_16_to_8(const wchar_t* wstr)
{
	return wstr_16_to_8_s(wstr);
}

WAOLASHD_API wchar_t* wstr_8_to_16(const char* str)
{
	return wstr_8_to_16_s(str);
}
