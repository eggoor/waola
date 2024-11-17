#pragma once

#include "include/IWaola.h"

namespace Waola {

	class Waola : public IWaola
	{
	public:
		void WakeUp(std::vector<IHostView*> hostList) override;
		
	public:
		Waola();
		~Waola() = default;

	private:
		Waola(const Waola&) = delete;
		Waola(Waola&&) = delete;
		Waola& operator=(const Waola&) = delete;
		Waola& operator=(Waola&&) = delete;	};
}
