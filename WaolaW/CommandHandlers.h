#pragma once

#include "HostView.h"

void OnCommandWaolaTask(int, HWND);
void OnCommandWakeUp();
void OnCommandWakeUpSelected();
void OnCommandExit(HWND);
void OnCommandAbout(HWND);
void OnCommandHostAdd(HWND);
void OnCommandHostCopy();
void OnCommandHostEdit(HWND);
void OnCommandHostDelete(HWND);
void OnSelectAllHosts();
void UpdateUI();
void DoUpdateUI(enum wtask);

static void DestroyHostViews(HostView**, uint32_t);
