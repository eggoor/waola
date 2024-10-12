#include "pch.h"

void swap(const void* restrict* const restrict l,
	const void* restrict* const restrict r)
{
	const void* t = *l;
	*l = *r;
	*r = t;
}

BOOL IsStringNullOrWhiteSpace(LPCTSTR restrict src)
{
	if (src) {
		while (*src && *src == _T(' ')) {
			++src;
		}
	}

	return !src || !*src;
}

LPTSTR StrDup(LPCTSTR restrict src)
{
	LPTSTR dup = NULL;

	if (!IsStringNullOrWhiteSpace(src)) {
		size_t buf_size = sizeof(TCHAR) * (_tcslen(src) + 1);
		dup = walloc(buf_size);
		if (dup) {
			_tcscpy_s(dup, buf_size / sizeof(TCHAR), src);
		}
		else {
			wlog_if_level(wll_warning, "No %zuB of free memory for string copy\n",
				buf_size);
		}
	}

	return dup;
}

void HandleError(BOOL messageBox)
{
	TCHAR szBuffer[DEF_STR_BUFFER_SIZE];
	LPTSTR pszMessage;

	DWORD dwError = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		(LPTSTR)&pszMessage, 0, NULL);

	if (!dwError)
	{
		_sntprintf_s(szBuffer, _countof(szBuffer), _TRUNCATE,
			TEXT("An error occurred calling FormatMessage().")
			TEXT("Error Code %d"), GetLastError());

		if (messageBox)
		{
			MessageBox(NULL, szBuffer, TEXT("Error"),
				MB_ICONSTOP | MB_ICONEXCLAMATION);
		}

		wlog_if_level(wll_error, "Unable to format error message: %d\n",
			dwError);

		return;
	}

	if (wlog_get_level() >= wll_error)
	{
		char* errorMessage = NULL;
#ifdef UNICODE
		errorMessage = wstr_16_to_8(pszMessage);
#else
		errorMessage = pszMessage;
#endif

		wlog(wll_error, errorMessage);

#ifdef UNICODE
		wfree(errorMessage);
#endif
	}

	if (messageBox)
	{
		MessageBox(NULL, pszMessage, TEXT("Error"), MB_ICONEXCLAMATION | MB_OK);
	}
}

void OpenURL(LPCTSTR pszUrl)
{
	ShellExecute(NULL, _T("open"), pszUrl, NULL, NULL, SW_SHOWNORMAL);
}
