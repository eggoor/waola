#include "pch.h"

#include "include/VaultEvent.h"

using namespace Waola;

VaultEvent::VaultEvent(VaultOperation opCode, const IHostView& hostView)
	: OpCode(opCode)
	, HostView(hostView)
{
}
