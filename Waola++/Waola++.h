#pragma once

#include "include/IWaola.h"

namespace Waola {

	class Waola : public IWaola
	{
	public:
		void WakeUp(std::vector<IHostView*> hostList) override;
	};
}
