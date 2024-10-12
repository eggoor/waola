#pragma once

void OnWmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
void OnWmWaolaStateChanged(wtask_t tasks);
void OnWmWaolaHostDiscovered(waolasc_opcode_t op, whost_view_t* hw);
void OnWmWaolaTaskFinished(HWND hWndMain, UINT_PTR rc);
BOOL OnWmSetCursor();
void OnWmCreate(HWND hWndMain);
void OnWmNotify(HWND hWndMain, WPARAM wParam, LPARAM lParam);
void OnWmSize(HWND hWndMain, WPARAM wParam, LPARAM lParam);
BOOL OnWmContextMenu(LPARAM lParam);
void OnWmClose();
void OnWmDestroy(HWND hWnd, WPARAM exitCode);
