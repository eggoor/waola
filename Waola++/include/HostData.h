/** @file HostData.h
* Waola host plain data without behavior.
*/

#pragma once

/*
Include in precompiled header:
#include <time.h>
#include <string>
*/

namespace Waola {

	class HostData
	{
	public:
		HostData(const char* displayName, const char* hostname,
			const char* ipAddr, const char* macAddr,
			time_t lastSeenOnline);
		~HostData();

	public:
		const char* DisplayName;
		const char* Hostname;
		const char* IpAddr;
		const char* MacAddr;
		const time_t LastSeenOnline;
	};
}
