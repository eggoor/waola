#pragma once

#include "HostView.h"

BOOL WaolawCreate();
void WaolawDestroy();

void WaolaWakeUp(HostView**, size_t);
void WaolawStartTask(int, HWND);
void WaolawCancelTask();
BOOL WaolaWaitForTaskFinish();
void WaolawOnTaskFinished(HWND);
wtask_t WaolawGetCurrentTasks();
void WaolawAddHostToModel(const whost_data_t*);
HostView* WaolawGetHost(int);
void WaolawUpdateHost(const HostView*);
void WaolawDeleteHosts(HostView**, uint32_t);
void WaolawDeleteHost(HostView*);
BOOL WaolawIsEmpty();
BOOL WaolawLoad();
int WaolawGetTaskId();
void WaolaProcessStateChanged(wtask_t);
void WaolaProcessHostDiscovered(waolava_op_t, whost_view_t*);

static void WaolawOnStateChanged(const waolasc_cbi_t* const);
static void WaolawOnHostDiscovered(const waolava_cbi_t* const);
static void AddHostToView(whost_view_t*);
static void RenumberHosts();
