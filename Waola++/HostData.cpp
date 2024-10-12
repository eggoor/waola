#include "pch.h"

#include "include/HostData.h"

using namespace Waola;

HostData::HostData(const char* displayName,
	const char* hostName, const char* ipAddr,
	const char* macAddr, time_t lastSeenOnline)
	: DisplayName(StrDup(displayName))
	, Hostname(StrDup(hostName))
	, IpAddr(StrDup(ipAddr))
	, MacAddr(StrDup(macAddr))
	, LastSeenOnline(lastSeenOnline)
{
}

HostData::~HostData()
{
	delete[] DisplayName;
	delete[] Hostname;
	delete[] IpAddr;
	delete[] MacAddr;
}
