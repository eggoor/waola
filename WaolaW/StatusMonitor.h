#pragma once

#define STATUS_BUFFER_SIZE 128

void StatusStartMonitoring(waolasc_t* scanner);
void StatusStopMonitoring();

BOOL IsStatusDataValid(const void* const restrict data,
	const char* warningMsg);
void ProcessIpAddress(uint32_t ipAddr, UINT nFormatId);

static void UpdateStatus(const waolasc_t* scanner);
static BOOL ProcessStatus(wmodule_t module,
	int opCode, const void* const restrict data);

static HANDLE DoStartMonitoring(waolasc_t* scanner);
static DWORD CALLBACK StatusMonitorProc(LPVOID lpParam);
