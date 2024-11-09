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
		
	public:
		virtual void WakeUp(std::vector<IHostView*> hostList) = 0;
		
	public:
		virtual ~IWaola() = default;

	protected:
		IWaola() = default;

	private:
		IWaola(const IWaola&) = delete;
		IWaola(IWaola&&) = delete;
		IWaola& operator=(const IWaola&) = delete;
		IWaola& operator=(IWaola&&) = delete;
	};
}
