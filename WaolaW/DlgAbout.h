#pragma once

INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
void InitTabCtrl(HWND hDlg);
void AdjustTabCtrlSelectedChildPos(HWND hwndTabCtrl);
void OnTabEvent(HWND hDlg, UINT opCode);
void OnRichEditEvent(const ENLINK* pLink);
void OnContextMenu(HWND hWnd, LPARAM lParam);