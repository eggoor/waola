#include "pch.h"

#include "Waola.h"

using namespace System;

WaolaCli::Waola::Waola()
	: waola(::Waola::IWaola::Create())
{

}

WaolaCli::Waola::~Waola()
{
	if (!disposed) {
		disposed = true;
		this->!Waola();
	}
}

WaolaCli::Waola::!Waola()
{
	::Waola::IWaola::Destroy(waola);
}

void WaolaCli::Waola::WakeUp(IReadOnlyList<WaolaCli::IHostView^>^ hostList)
{
	std::vector<::Waola::IHostView*> nativeHostList(hostList->Count);

	int i = 0;
	for each (auto hostView in hostList) {
		nativeHostList[i++] = hostView->Native;
	}

	try {
		waola->WakeUp(nativeHostList);
	}
	catch (const std::runtime_error& ex) {
		throw gcnew System::Exception(gcnew String(ex.what()));
	}
}
