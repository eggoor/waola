#pragma once

#include "include/IWaola.h"

namespace Waola {

	class Waola : public IWaola
	{
	public:
		void WaolaH(std::vector<IHostView*> hostList) override;
	};
}
