#include "pch.h"

#include "Resource.h"

#include "RichEdit.h"

extern HINSTANCE g_hInst;

static LPCTSTR g_pszTabChildCtrlClass = MSFTEDIT_CLASS;

HWND RichEditMake(HWND hwndParent, INT_PTR childId, LPCTSTR lpWindowName, DWORD dwStyle)
{
	HWND hwndRichEdit = CreateWindowEx(0, g_pszTabChildCtrlClass, lpWindowName, dwStyle,
		0, 0, 0, 0, hwndParent, (HMENU)childId, g_hInst, 0);

	LRESULT result = SNDMSG(hwndRichEdit, EM_SETEDITSTYLE,
		SES_HYPERLINKTOOLTIPS, SES_HYPERLINKTOOLTIPS);

	LRESULT mask = SNDMSG(hwndRichEdit, EM_GETEVENTMASK, 0, 0);
	SNDMSG(hwndRichEdit, EM_SETEVENTMASK, 0, mask | ENM_LINK);
	SNDMSG(hwndRichEdit, EM_AUTOURLDETECT, TRUE, 0);

	return hwndRichEdit;
}

BOOL FillRichEditFromResource(HWND hWnd, DWORD id)
{
	BOOL fSuccess = FALSE;

	resource_data_t resource_data = { 0 };

	if (!RichEditLoadFileResource(id, IDR_RTF, &resource_data))
	{
		return FALSE;
	}

	EDITSTREAM es = {
		.pfnCallback = RichEditStreamCallback,
		.dwCookie = (DWORD_PTR)&resource_data
	};

	if (SNDMSG(hWnd, EM_STREAMIN, SF_RTF, (LPARAM)&es) && es.dwError == 0)
	{
		return TRUE;
	}

	return FALSE;
}

DWORD CALLBACK RichEditStreamCallback(DWORD_PTR dwCookie,
	LPBYTE lpBuff, LONG cb, PLONG pcb)
{
	resource_data_t* pData = (resource_data_t*)dwCookie;

	*pcb = min(cb, pData->nSize - pData->nRead);
	if (*pcb > 0)
	{
		memcpy(lpBuff, pData->pData + pData->nRead, *pcb);
		pData->nRead += *pcb;
	}

	return 0;
}

BOOL RichEditLoadFileResource(DWORD id, DWORD type, resource_data_t* pData)
{
	if (!pData)
	{
		wlog_if_level(wll_error, "%s: pData == %p\n",
			__func__, pData);
		return FALSE;
	}

	pData->pData = NULL;

	HRSRC hR = FindResource(g_hInst, MAKEINTRESOURCE(id),
		MAKEINTRESOURCE(type));
	if (!hR)
	{
		HandleError(FALSE);
		return FALSE;
	}

	HGLOBAL rcData = LoadResource(g_hInst, hR);
	if (!rcData)
	{
		HandleError(FALSE);
		return FALSE;
	}

	pData->nSize = SizeofResource(g_hInst, hR);
	if (0 == pData->nSize)
	{
		HandleError(FALSE);
		return FALSE;
	}

	pData->pData = LockResource(rcData);
	if (!pData->pData)
	{
		HandleError(FALSE);
		return FALSE;
	}

	return TRUE;
}

LPTSTR RichEditGetTextRange(HWND hWnd, CHARRANGE range)
{
	const size_t bufSize = sizeof(TCHAR)* (range.cpMax - range.cpMin + 1);
	LPTSTR szTextRangeBuffer = walloc(bufSize);

	TEXTRANGE tr = {
		.chrg = range,
		.lpstrText = szTextRangeBuffer
	};

	LRESULT rc = SNDMSG(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

	if (rc > 0)
	{
		RichEditRemoveHyperlinkTag(szTextRangeBuffer);
	}
	else
	{
		szTextRangeBuffer[0] = _T('\0');
	}

	return szTextRangeBuffer;
}

LPTSTR RichEditGetSelectedText(HWND hWnd)
{
	CHARRANGE range = { 0 };

	SNDMSG(hWnd, EM_EXGETSEL, 0, (LPARAM)&range);
	
	return RichEditGetTextRange(hWnd, range);
}

void RichEditRemoveHyperlinkTag(LPTSTR pszText)
{
	// HYPERLINK "https://www.paypal.com/ua/home"egggor@gmail.com
	static LPCTSTR pszHyperlinkTag = _T("HYPERLINK");
	static size_t nHyperlinkTagLen = 9;

	if (_tcsncmp(pszText, pszHyperlinkTag, nHyperlinkTagLen) == 0) {
		LPCTSTR pszAfterHyperlink = pszText + nHyperlinkTagLen + 2;
		while (pszAfterHyperlink && *pszAfterHyperlink++ != _T('"'));
		while (pszAfterHyperlink) {
			*pszText++ = *pszAfterHyperlink++;
		}
		*pszText = _T('\0');
	}
}