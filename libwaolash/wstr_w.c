#include "pch.h"

#include "include/wmem_s.h"

char* wstr_16_to_8_s(const wchar_t* const restrict wstr)
{	
	char* str = NULL;

	if (!wstr) {
		goto end;
	}

	int buf_size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

	if (!buf_size) {
		wlog_if_level(wll_warning, "WideCharToMultiByte returned 0\n");
		goto end;
	}

	str = walloc_s(buf_size);

	if (!str) {
		wlog_if_level(wll_warning, "No %zuB of free memory for multy byte string\n",
			buf_size);
		goto end;
	}

	buf_size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, buf_size, NULL, NULL);

	if (0 == buf_size) {
		wlog_if_level(wll_warning, "WideCharToMultiByte failed\n");
		wfree_s(str);
		str = NULL;
	}

end:
	return str;
}

wchar_t* wstr_8_to_16_s(const char* const restrict str)
{
	wchar_t* wstr = NULL;

	if (!str) {
		goto end;
	}

	int max_char = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);

	if (!max_char) {
		wlog_if_level(wll_warning, "MultiByteToWideChar returned 0\n");
		goto end;
	}

	wstr = walloc_s(sizeof(wchar_t) * max_char);

	if (!wstr) {
		wlog_if_level(wll_warning, "No %zuB of free memory for wide char string\n",
			sizeof(wchar_t) * max_char);
		goto end;
	}

	max_char = MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, max_char);

	if (0 == max_char) {
		wlog_if_level(wll_warning, "MultiByteToWideChar failed\n");
		wfree_s(wstr);
		wstr = NULL;
	}

end:
	return wstr;
}

const char* wstrerror_s(int err)
{
#define BUFF_LEN 64
	static char buffer[BUFF_LEN];
	strerror_s(buffer, BUFF_LEN, err);

	return buffer;
}
