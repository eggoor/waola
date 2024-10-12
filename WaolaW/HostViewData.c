#include "pch.h"

#include "HostViewData.h"

void HostViewDataFree(HostViewData* viewData)
{
	wfree(viewData->pszDisplayName);
	viewData->pszDisplayName = NULL;
	wfree(viewData->pszHostname);
	viewData->pszHostname = NULL;
	wfree(viewData->pszIpAddr);
	viewData->pszIpAddr = NULL;
	wfree(viewData->pszMacAddr);
	viewData->pszMacAddr = NULL;
}

BOOL HostViewDataIsEmpty(HostViewData* viewData)
{
	return !viewData->pszDisplayName && !viewData->pszHostname
		&& !viewData->pszIpAddr && !viewData->pszMacAddr
		&& 0 == viewData->tLastSeenOnline;
}