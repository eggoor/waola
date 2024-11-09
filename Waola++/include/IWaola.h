/** @file IWaola.h
* Waola C++ wrapper for WoL functionality.
*/

#pragma once

#include "IHostView.h"

namespace Waola {

	class IWaola
	{
	public:
		static IWaola* Create();
		static void Destroy(IWaola* const waola);
		
		virtual void WakeUp(std::vector<IHostView*> hostList) = 0;
		virtual ~IWaola() {}
	};
}
