#pragma once

#include <IScanner.h>

#include "IScanner.h"
#include "OpCodeScanner.h"
#include "OpCodeResolver.h"
#include "OpCodeDiscoverer.h"

using System::Runtime::InteropServices::GCHandle;

namespace WaolaCli {

	public ref class Scanner : IScanner
	{
	public:
		Scanner();
		~Scanner();
		!Scanner();

		// IScanner implementation
		virtual property bool IsVaultEmpty { bool get(); };
		virtual property WaolaTask CurrentTasks { WaolaTask get(); };
		virtual property IReadOnlyList<IHostView^>^ Hosts { IReadOnlyList<IHostView^>^ get(); };
		virtual void SetLogLevel(LogLevel logLevel);
		virtual void SetMaxThreads(unsigned int maxThreads);
		virtual void Discover();
		virtual Task^ DiscoverAsync();
		virtual void Refresh();
		virtual Task^ RefreshAsync();
		virtual void RefreshHost(IHostView^ hostView);
		virtual void Add(HostData^ hostData);
		virtual Task^ AddAsync(HostData^ hostData);
		virtual void BatchAdd(IReadOnlyList<HostData^>^ hostDataList);
		virtual void DeleteHost(IHostView^ hostView);
		virtual void CancelTask();
		virtual Object^ GetStatus(Module% module, int% opCode);
		virtual WaolaTask GetCurrentTasks();
		virtual event DelegateStateEvent^ StateEvent;
		virtual event DelegateVaultEvent^ VaultEvent;

	private:
		delegate void VaultEventDelegate(const Waola::VaultEvent&);
		void OnVaultEvent(const Waola::VaultEvent&);
		Waola::VaultEventCallback MakeVaultEventCallback();
		GCHandle vaultEventCallbackGcHandle;
		Waola::VaultUnsubscribeToken* vaultUnsubscribeToken;
		void FireVaultEvent(VaultEventArgs^ vaultEventArgs);

		delegate void StateEventDelegate(const Waola::StateEvent&);
		void OnStateEvent(const Waola::StateEvent&);
		Waola::StateEventCallback MakeStateEventCallback();
		GCHandle stateEventCallbackGcHandle;
		Waola::StateUnsubscribeToken* stateUnsubscribeToken;
		void FireStateEvent(StateEventArgs^ stateEventArgs);
		
		Object^ StatusDataToObject(Module module, int opCode,
			const void* statusData);
		Object^ DiscovererStatusDataToObject(OpCodeDiscoverer opCode,
			const void* statusData);
		Object^ ResolverStatusDataToObject(OpCodeResolver opCode,
			const void* statusData);

	private:
		Waola::IScanner* scanner;
		bool disposed;
	};
}
