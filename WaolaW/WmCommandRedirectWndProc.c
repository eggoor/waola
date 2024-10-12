#include "pch.h"

#include "WmCommandRedirectWndProc.h"

LRESULT CALLBACK WmCommandRedirectWndProc(const HWND hWnd, const UINT msg,
	const WPARAM wParam, const LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		SendMessage(GetParent(hWnd), msg, wParam, lParam);
		return 0;
	default:
		return CallWindowProc((WNDPROC)GetWindowLongPtr(hWnd,
			GWLP_USERDATA), hWnd, msg, wParam, lParam);
	}
}
