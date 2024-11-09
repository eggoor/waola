/** @file VaultEvent.h
* Waola vault opearation event.
*/

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
		
	public:
		~VaultEvent() = default;

	private:
		VaultEvent(const VaultEvent&) = delete;
		VaultEvent(VaultEvent&&) = delete;
		VaultEvent& operator=(const VaultEvent&) = delete;
		VaultEvent& operator=(VaultEvent&&) = delete;
	};
}
