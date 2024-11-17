#pragma once

#include "WaolaTask.h"

using System::EventArgs;

namespace WaolaCli {

	public ref class StateEventArgs : public EventArgs
	{
	public:
		StateEventArgs(const Waola::StateEvent& statusEvent);

	property WaolaTask Tasks { WaolaTask get(); private: void set(WaolaTask); }

	private:
		WaolaTask tasks;
	};
}
