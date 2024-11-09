#pragma once

#include "VaultOperation.h"
#include "IHostView.h"
#include "EventBase.h"

namespace Waola {

	class VaultEvent: public EventBase
	{
	public:
		VaultEvent(VaultOperation OpCode, const IHostView& hostView);

	public:
		const VaultOperation OpCode;
		const IHostView& HostView;
	};
}
