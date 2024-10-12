#pragma once

#include "VaultOperation.h"
#include "IHostView.h"

namespace Waola {

	class VaultEvent
	{
	public:
		VaultEvent(VaultOperation OpCode, const IHostView& hostView);

	public:
		const VaultOperation OpCode;
		const IHostView& HostView;
	};
}
