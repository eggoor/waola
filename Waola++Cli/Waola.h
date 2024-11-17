#pragma once

#include "IWaola.h"

namespace WaolaCli {

	public ref class Waola : IWaola
	{
	public:
		Waola();
		~Waola();
		!Waola();

	public:
		// IWaola implementation
		virtual void WakeUp(IReadOnlyList<WaolaCli::IHostView^>^ hostList);

	private:
		::Waola::IWaola* waola;
		bool disposed;
	};
}
