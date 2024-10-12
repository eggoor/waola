#pragma once

#include "ListView.h"

static void Init();
static void PositionHeader();
static void SetSortIcon(const int, int);
static void DeleteHost(HostView*);
static void OnColumnClick(NMLISTVIEW*);
static void OnItemChanged(NMLISTVIEW*);
static void OnItemDoucleclicked(LPNMITEMACTIVATE);

static int CALLBACK CompareProc(LPARAM, LPARAM, LPARAM);