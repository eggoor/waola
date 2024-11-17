#pragma once

#include "HostFiled.h"
#include "IHostView.h"

namespace WaolaCli {

	public ref class HostView : public IHostView
	{
	public:
		HostView(const Waola::IHostView& hostView);
		~HostView();
		!HostView();

		// IHostView implementation
		virtual property IntPtr Id { IntPtr get(); }
		virtual property String^ HumanReadableId { String^ get(); }
		virtual property String^ DisplayName { String^ get(); void set(String^); }
		virtual property String^ DirectDisplayName { String^ get(); }
		virtual property String^ Hostname { String^ get(); void set(String^); }
		virtual property String^ IpAddressString { String^ get(); void set(String^); }
		virtual property String^ MacAddressString { String^ get(); void set(String^); }
		virtual property String^ LastSeenOnlineString { String^ get(); }
		virtual property array<System::Byte>^ MacAddress { array<System::Byte>^ get(); }
		virtual property DateTime LastSeenOnline { DateTime get(); }
		virtual property HostChangedField State { HostChangedField get(); }
		virtual property OpResult Result { OpResult get(); }

		virtual property Waola::IHostView* Native { Waola::IHostView* get(); }

		String^ ToString() override;

		virtual void SetFieldChangeProcessed(HostChangedField hostField);

		static HostChangedField HostFieldToChangedField(HostField hostField);

		// Inherited via IHostView
		bool Equals(Object^ obj) override;
		virtual bool Equals(IHostView^ other);
		int GetHashCode() override;

	private:
		Waola::IHostView* hostView;
		bool disposed;
	};
}
