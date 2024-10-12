#pragma once

typedef struct resource_data {
    const char* pData;
    LONG nRead;
    LONG nSize;
} resource_data_t;

HWND RichEditMake(HWND hwndParent, INT_PTR childId,
    LPCTSTR lpWindowName, DWORD dwStyle);
BOOL FillRichEditFromResource(HWND hWnd, DWORD id);
DWORD CALLBACK RichEditStreamCallback(DWORD_PTR dwCookie,
    LPBYTE lpBuff, LONG cb, PLONG pcb);
BOOL RichEditLoadFileResource(DWORD id, DWORD type,
    resource_data_t* pData);
LPTSTR RichEditGetTextRange(HWND hWnd, CHARRANGE range);
LPTSTR RichEditGetSelectedText(HWND hWnd);
void RichEditRemoveHyperlinkTag(LPTSTR pszText);