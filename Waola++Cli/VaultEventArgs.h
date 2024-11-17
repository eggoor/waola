#pragma once

#include "VaultOperation.h"

#include "IHostView.h"

using System::EventArgs;

namespace WaolaCli {

	public ref class VaultEventArgs : public EventArgs
	{
	public:
		VaultEventArgs(const Waola::VaultEvent& VaultEvent);

		property VaultOperation OpCode { VaultOperation get(); private: void set(VaultOperation); }
		property IHostView^ HostView { IHostView^ get(); private: void set(IHostView^); }

	private:
		VaultOperation opCode;
		IHostView^ hostView;
	};
}