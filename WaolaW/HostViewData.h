#pragma once

typedef struct THostViewData
{
	LPTSTR pszDisplayName;
	LPTSTR pszHostname;
	LPTSTR pszIpAddr;
	LPTSTR pszMacAddr;
	time_t tLastSeenOnline;
} HostViewData;

void HostViewDataFree(HostViewData* viewData);
BOOL HostViewDataIsEmpty(HostViewData* viewData);
