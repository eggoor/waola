#include "pch.h"

#include "Clipboard.h"

BOOL WINAPI ClipboardCopy(LPCTSTR pszData)
{
	if (!OpenClipboard(NULL))
	{
		return FALSE;
	}

	EmptyClipboard();

	const size_t bataLen = _tcslen(pszData);
	const size_t bufSize = sizeof(TCHAR) * (bataLen + 1);

	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, bufSize);

	if (!hglbCopy)
	{
		CloseClipboard();
		return FALSE;
	}

	LPTSTR lpCopy = GlobalLock(hglbCopy);
	if (!lpCopy)
	{
		CloseClipboard();
		return FALSE;
	}

	memcpy(lpCopy, pszData, bufSize);
	GlobalUnlock(hglbCopy);

	UINT nFormat = 0;
#ifdef UNICODE
	nFormat = CF_UNICODETEXT;
#else
	nFormat = CF_TEXT;
#endif
	SetClipboardData(nFormat, hglbCopy);

	CloseClipboard();

	return TRUE;
}
