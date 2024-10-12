#include "pch.h"

#include "StringExtensions.h"
#include "HostView.h"
#include "Future2TaskHelper.h"
#include "Future2Task.h"

#include "Scanner.h"

using namespace WaolaCli;

using System::Runtime::InteropServices::Marshal;
using namespace System::ComponentModel;

Scanner::Scanner()
	: scanner(Waola::IScanner::Create())
	, stateUnsubscribeToken(new Waola::StateUnsubscribeToken)
	, vaultUnsubscribeToken(new Waola::VaultUnsubscribeToken)
{
	try {
		auto stateEventCallback = MakeStateEventCallback();
		*stateUnsubscribeToken = scanner->SubscribeForStateEvents(stateEventCallback);

		auto vaultEventCallback = MakeVaultEventCallback();
		*vaultUnsubscribeToken = scanner->SubscribeForVaultEvents(vaultEventCallback);
	}
	catch (const std::runtime_error& ex) {
		throw gcnew System::Exception(gcnew String(ex.what()));
	}
}

Scanner::~Scanner()
{
	if (!disposed) {
		disposed = true;
		this->!Scanner();
	}
}

Scanner::!Scanner()
{
	scanner->UnubscribeFromVaultEvents(*vaultUnsubscribeToken);
	delete vaultUnsubscribeToken;

	if (vaultEventCallbackGcHandle.IsAllocated) {
		vaultEventCallbackGcHandle.Free();
	}

	scanner->UnubscribeFromStateEvents(*stateUnsubscribeToken);
	delete stateUnsubscribeToken;

	if (stateEventCallbackGcHandle.IsAllocated) {
		stateEventCallbackGcHandle.Free();
	}

	Waola::IScanner::Destroy(scanner);
}

bool Scanner::IsVaultEmpty::get()
{
	return scanner->IsVaultEmpty();
}

WaolaCli::WaolaTask Scanner::CurrentTasks::get()
{
	return (WaolaTask)scanner->GetCurrentTasks();
}

IReadOnlyList<IHostView^>^ Scanner::Hosts::get()
{
	auto hostList = gcnew List<IHostView^>();

	auto hostListNative = scanner->GetHosts();

	for (const auto& hostViewNative : hostListNative) {
		hostList->Add(gcnew HostView(*hostViewNative));
		delete hostViewNative;
	}

	return hostList;
}

void Scanner::SetLogLevel(LogLevel logLevel)
{
	scanner->SetLogLevel((Waola::LogLevel)logLevel);
}

void Scanner::SetMaxThreads(unsigned int maxThreads)
{
	scanner->SetMaxThreads(maxThreads);
}

void Scanner::Discover()
{
	try {
		scanner->DiscoverAsync();
	}
	catch (const std::runtime_error& ex) {
		throw gcnew System::Exception(gcnew String(ex.what()));
	}
}

Task^ Scanner::DiscoverAsync()
{
	try {
		return Feature2Task<std::future<void>(Waola::IScanner::*)(),
			Feature2TaskHelper<void, void>>(&Waola::IScanner::Discover, scanner);
	}
	catch (const std::runtime_error& ex) {
		throw gcnew System::Exception(gcnew String(ex.what()));
	}
}

void Scanner::Refresh()
{
	try {
		scanner->Refresh();
	}
	catch (const std::runtime_error& ex) {
		throw gcnew System::Exception(gcnew String(ex.what()));
	}
}

Task^ Scanner::RefreshAsync()
{
	try {
		return Feature2Task<std::future<void>(Waola::IScanner::*)(),
		Feature2TaskHelper<void, void>>(&Waola::IScanner::Refresh, scanner);
	}
	catch (const std::runtime_error& ex) {
		throw gcnew System::Exception(gcnew String(ex.what()));
	}
}

void Scanner::RefreshHost(IHostView^ hostView)
{
	scanner->RefreshHost(hostView->Native);
}

void Scanner::Add(HostData^ hostData)
{
	const char* displayName = StringExtensions::ToUtf8(hostData->DisplayName);
	const char* hostName = StringExtensions::ToUtf8(hostData->Hostname);
	const char* ipAddr = StringExtensions::ToUtf8(hostData->IpAddr);
	const char* macAddr = StringExtensions::ToUtf8(hostData->MacAddr);
	time_t lastSeenOnline = hostData->LastSeenOnline > DateTime::MinValue
		? (time_t)(hostData->LastSeenOnline - DateTime(1970, 1, 1)).TotalSeconds
		: 0;

	Waola::HostData nativeHostData(displayName, hostName,
		ipAddr, macAddr, lastSeenOnline);

	delete[] macAddr;
	delete[] ipAddr;
	delete[] hostName;
	delete[] displayName;

	try {
		scanner->Add(nativeHostData);
	}
	catch (const std::runtime_error& ex) {
		throw gcnew System::Exception(gcnew String(ex.what()));
	}
}

Task^ Scanner::AddAsync(HostData^ hostData)
{
	const char* displayName = StringExtensions::ToUtf8(hostData->DisplayName);
	const char* hostName = StringExtensions::ToUtf8(hostData->Hostname);
	const char* ipAddr = StringExtensions::ToUtf8(hostData->IpAddr);
	const char* macAddr = StringExtensions::ToUtf8(hostData->MacAddr);
	time_t lastSeenOnline = hostData->LastSeenOnline > DateTime::MinValue
		? (time_t)(hostData->LastSeenOnline - DateTime(1970, 1, 1)).TotalSeconds
		: 0;

	Waola::HostData nativeHostData(displayName, hostName, ipAddr, macAddr, lastSeenOnline);

	delete[] macAddr;
	delete[] ipAddr;
	delete[] hostName;
	delete[] displayName;

	try {
		return Feature2Task<std::future<void>(Waola::IScanner::*)(const Waola::HostData&),
			Feature2TaskHelper<void, void>>(&Waola::IScanner::Add, scanner, nativeHostData);
	}
	catch (const std::runtime_error& ex) {
		throw gcnew System::Exception(gcnew String(ex.what()));
	}
}

void Scanner::BatchAdd(IReadOnlyList<HostData^>^ hostDataList)
{
	std::vector<const Waola::HostData*> nativeHostDataList;

	for each (auto hostData in hostDataList)
	{
		const char* displayName = StringExtensions::ToUtf8(hostData->DisplayName);
		const char* hostName = StringExtensions::ToUtf8(hostData->Hostname);
		const char* ipAddr = StringExtensions::ToUtf8(hostData->IpAddr);
		const char* macAddr = StringExtensions::ToUtf8(hostData->MacAddr);
		auto lsoUnixEpoch = hostData->LastSeenOnline - DateTime(1970, 1, 1);
		time_t lastSeenOnline = (time_t)lsoUnixEpoch.TotalSeconds;

		nativeHostDataList.emplace_back(new Waola::HostData(displayName,
			hostName, ipAddr, macAddr, lastSeenOnline));

		delete[] macAddr;
		delete[] ipAddr;
		delete[] hostName;
		delete[] displayName;
	}

	scanner->BatchAdd(nativeHostDataList);

	for (auto& hostData : nativeHostDataList) {
		delete hostData;
	}
}

void Scanner::DeleteHost(IHostView^ hostView)
{
	scanner->DeleteHost(hostView->Native);
}

void Scanner::CancelTask()
{
	scanner->CancelTask();
}

void Scanner::OnVaultEvent(const Waola::VaultEvent& vaultEvent)
{
	FireVaultEvent(gcnew VaultEventArgs(vaultEvent));
}

void Scanner::FireVaultEvent(VaultEventArgs^ vaultEventArgs)
{
	VaultEvent(this, vaultEventArgs);
}

void Scanner::OnStateEvent(const Waola::StateEvent& StateEvent)
{
	FireStateEvent(gcnew StateEventArgs(StateEvent));
}

void Scanner::FireStateEvent(StateEventArgs^ stateEventArgs)
{
	StateEvent(this, stateEventArgs);
}

Waola::VaultEventCallback Scanner::MakeVaultEventCallback()
{
	auto vaultEventDelegate = gcnew VaultEventDelegate(this, &Scanner::OnVaultEvent);
	vaultEventCallbackGcHandle = GCHandle::Alloc(vaultEventDelegate);

	return Waola::VaultEventCallback((void(__cdecl*)(const Waola::VaultEvent&))
		Marshal::GetFunctionPointerForDelegate(vaultEventDelegate).ToPointer());
}

Waola::StateEventCallback Scanner::MakeStateEventCallback()
{
	auto stateEventDelegate = gcnew StateEventDelegate(this, &Scanner::OnStateEvent);
	stateEventCallbackGcHandle = GCHandle::Alloc(stateEventDelegate);

	return Waola::StateEventCallback((void(__cdecl*)(const Waola::StateEvent&))
		Marshal::GetFunctionPointerForDelegate(stateEventDelegate).ToPointer());
}

WaolaTask Scanner::GetCurrentTasks()
{
	return (WaolaTask)scanner->GetCurrentTasks();
}

Object^ Scanner::GetStatus(Module% module, int% opCode)
{
	Waola::Module nativeModule;
	int nativeOpCode;
	Object^ statusData = nullptr;

	const void* statusDataNative = scanner->GetStatus(&nativeModule, &nativeOpCode);

	module = (Module)nativeModule;
	opCode = nativeOpCode;

	if (Waola::wm_undefined != nativeModule)
	{
		statusData = StatusDataToObject(module, opCode, statusDataNative);
	}

	return statusData;
}

Object^ Scanner::StatusDataToObject(Module module, int opCode, const void* const statusData)
{
	Object^ object = nullptr;

	switch (module)
	{
	case Module::Scanner:
		// Do nothing
		break;
	case Module::Discoverer:
		object = DiscovererStatusDataToObject((OpCodeDiscoverer)opCode, statusData);
		break;
	case Module::Resolver:
		object = ResolverStatusDataToObject((OpCodeResolver)opCode, statusData);
		break;
	default:
		throw gcnew InvalidEnumArgumentException(nameof(module),
			(int)module, module.GetType());
	}

	return object;
}

Object^ Scanner::DiscovererStatusDataToObject(OpCodeDiscoverer opCode,
	const void* statusData)
{
	Object^ object = nullptr;
	
	switch (opCode)
	{
	case OpCodeDiscoverer::SchedulingArp:
	case OpCodeDiscoverer::SendingArp:
	case OpCodeDiscoverer::GotArp:
	case OpCodeDiscoverer::ArpFailed:
		object = gcnew UInt32(*(uint32_t*)statusData);
		break;
	case OpCodeDiscoverer::Undefined:
	default:
		throw gcnew InvalidEnumArgumentException(nameof(opCode),
			(int)opCode, opCode.GetType());
	}

	return object;
}

Object^ Scanner::ResolverStatusDataToObject(OpCodeResolver opCode,
	const void* statusData)
{
	Object^ object = nullptr;
	
	switch (opCode)
	{
	case OpCodeResolver::GetAddrInfo:
	case OpCodeResolver::GetAddrInfoFailed:
	case OpCodeResolver::GotNameInfo:
		object = StringExtensions::FromUtf8(reinterpret_cast<const char*>(statusData));
		break;
	case OpCodeResolver::GotAddrInfo:
	case OpCodeResolver::GetNameInfo:
	case OpCodeResolver::GetNameInfoFailed:
	case OpCodeResolver::SendingArp:
	case OpCodeResolver::GotArp:
	case OpCodeResolver::ArpFailed:
	{
		object = gcnew UInt32(*(uint32_t*)statusData);
	}
	break;
	case OpCodeResolver::Undefined:
	default:
		throw gcnew InvalidEnumArgumentException(nameof(opCode),
			(int)opCode, opCode.GetType());
	}

	return object;
}
