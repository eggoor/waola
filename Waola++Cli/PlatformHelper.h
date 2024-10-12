#pragma once

namespace WaolaCli {

	public ref class PlatformHelper
	{
	public:
		static property bool Is32BitProcess { bool get(); };
	};
}