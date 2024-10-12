#pragma once

using namespace System;

namespace WaolaCli {

	public ref class HostData
	{
	public:
		HostData();

		HostData(String^ displayName, String^ hostName,
			String^ ipAddr, String^ macAddr,
			DateTime lastSeenOnline);

		property String^ DisplayName { String^ get(); private: void set(String^); }
		property String^ Hostname { String^ get(); private: void set(String^); }
		property String^ IpAddr { String^ get(); private: void set(String^); }
		property String^ MacAddr { String^ get(); private: void set(String^); }
		property DateTime LastSeenOnline { DateTime get(); private: void set(DateTime); };

	private:
		String^ displayName;
		String^ hostName;
		String^ ipAddr;
		String^ macAddr;
		DateTime lastSeenOnline;
	};
}
