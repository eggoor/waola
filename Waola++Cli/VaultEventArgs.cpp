#include "pch.h"

#include "HostView.h"

#include "VaultEventArgs.h"

using namespace WaolaCli;

VaultEventArgs::VaultEventArgs(const Waola::VaultEvent& VaultEvent)
{
	OpCode = (VaultOperation)VaultEvent.OpCode;
	HostView = gcnew WaolaCli::HostView(VaultEvent.HostView);
}

VaultOperation VaultEventArgs::OpCode::get()
{
	return opCode;
}

void VaultEventArgs::OpCode::set(VaultOperation opCode)
{
	this->opCode = opCode;
}

IHostView^ VaultEventArgs::HostView::get()
{
	return hostView;
}

void VaultEventArgs::HostView::set(IHostView^ hostView)
{
	this->hostView = hostView;
}
