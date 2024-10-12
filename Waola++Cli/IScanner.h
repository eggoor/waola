#pragma once

#include "WaolaTask.h"
#include "LogLevel.h"
#include "IHostView.h"
#include "HostData.h"
#include "Module.h"
#include "StateEventArgs.h"
#include "VaultEventArgs.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

namespace WaolaCli {

	public delegate void DelegateStateEvent(Object^ sender,
		StateEventArgs^ stateEventArgs);
	public delegate void DelegateVaultEvent(Object^ sender,
		VaultEventArgs^ vaultEventArgs);

	public interface class IScanner : public IDisposable
	{
	public:
		property bool IsVaultEmpty { bool get(); }
		property WaolaTask CurrentTasks { WaolaTask get(); }
		property IReadOnlyList<IHostView^>^ Hosts { IReadOnlyList<IHostView^>^ get(); }
		void SetLogLevel(LogLevel logLevel);
		void SetMaxThreads(unsigned int maxThreads);
		void Discover();
		Task^ DiscoverAsync();
		void Refresh();
		Task^ RefreshAsync();
		void RefreshHost(IHostView^ hostView);
		void Add(HostData^ hostData);
		Task^ AddAsync(HostData^ hostData);
		void BatchAdd(IReadOnlyList<HostData^>^ hostDataList);
		void DeleteHost(IHostView^ hostView);
		void CancelTask();
		Object^ GetStatus(Module% module, int% opCode);
		WaolaTask GetCurrentTasks();

		event DelegateStateEvent^ StateEvent;
		event DelegateVaultEvent^ VaultEvent;
	};
}
