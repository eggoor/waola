#pragma once

#include "WaolaW.h"
#include "HostViewData.h"
#include "PermanentStorageCbd.h"

void SaveHosts(HDPA hostCollection);
HDPA LoadHosts();
void SaveWindowPos(const RECT* rcPos);
BOOL LoadWindowPos(RECT* rcPos);

static VS_FIXEDFILEINFO* GetVersionInfo(LPVOID* pResCopyBuffer);
static void SaveHost(HKEY hHosts, int nNumber, const HostView* const restrict hostView);
static whost_data_t* LoadHost(HKEY hHosts, LPCTSTR pszHostId);
static void EnumHosts(PermanentStorageCb cb, PermanentStorageCbd* const restrict cbd);
static void AddHostCb(PermanentStorageCbd* const restrict cbd);
static void DeleteHostCb(PermanentStorageCbd* const restrict cbd);
