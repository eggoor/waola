#pragma once

#include "HostField.h"
#include "HostChangedField.h"
#include "OpResult.h"

using namespace System;

namespace WaolaCli {

	public interface class IHostView
		: public System::IEquatable<IHostView^>
		, public System::IDisposable
	{
	public:
		property IntPtr Id { IntPtr get(); }
		property String^ HumanReadableId { String^ get(); }
		property String^ DisplayName { String^ get(); void set(String^); }
		property String^ DirectDisplayName { String^ get(); }
		property String^ Hostname { String^ get(); void set(String^); }
		property String^ IpAddressString { String^ get(); void set(String^); }
		property String^ MacAddressString { String^ get(); void set(String^); }
		property String^ LastSeenOnlineString { String^ get(); }
		property array<System::Byte>^ MacAddress { array<System::Byte>^ get(); }
		property DateTime LastSeenOnline { DateTime get(); }
		property HostChangedField State { HostChangedField get(); }
		property OpResult Result { OpResult get(); }

		property Waola::IHostView* Native { Waola::IHostView* get(); }

		String^ ToString();

		void SetFieldChangeProcessed(HostChangedField hostField);

		bool Equals(Object^ obj);
		bool Equals(IHostView^ other);
		int GetHashCode();
	};
}
