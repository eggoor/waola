#pragma once

#include "HostViewData.h"

INT_PTR CALLBACK HostEditDlgProc(HWND, UINT, WPARAM, LPARAM);

static void OnWmInitDialog(HWND hDlg, HostViewData* const restrict data);
static void LoadData(HWND hDlg);
static void SaveData(HWND hDlg);