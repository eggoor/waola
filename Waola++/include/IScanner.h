#pragma once

/*
 Include in precompiled header:
 #include <functional>
 #include <forward_list>
 #include <future>
 */

#include "Task.h"
#include "LogLevel.h"
#include "IHostView.h"
#include "HostData.h"
#include "Module.h"
#include "VaultEvent.h"
#include "StateEvent.h"

namespace Waola {

	typedef std::function<void(const StateEvent&)> StateEventCallback;
	typedef std::forward_list<StateEventCallback>::iterator StateUnsubscribeToken;

	typedef std::function<void(const VaultEvent&)> VaultEventCallback;
	typedef std::forward_list<VaultEventCallback>::iterator VaultUnsubscribeToken;

	class IScanner
	{
	public:
		static IScanner* Create();
		static void Destroy(IScanner* const scanner);

	public:
		virtual bool IsVaultEmpty() const = 0;
		virtual Task GetCurrentTasks() const = 0;
		virtual std::forward_list<IHostView*> GetHosts() = 0;
		virtual void SetLogLevel(LogLevel logLevel) = 0;
		virtual void SetMaxThreads(uint32_t maxThreads) = 0;
		virtual std::future<void> Discover() = 0;
		virtual void DiscoverAsync() = 0;
		virtual std::future<void> Refresh() = 0;
		virtual void RefreshHost(IHostView* const hostView) = 0;
		virtual std::future<void> Add(const HostData& hostData) = 0;
		virtual void BatchAdd(std::vector<const HostData*> hostDataList) = 0;
		virtual void DeleteHost(IHostView* const hostView) = 0;
		virtual void CancelTask() = 0;
		virtual const void* GetStatus(Module* module, int* const opCode) = 0;

		virtual StateUnsubscribeToken SubscribeForStateEvents(StateEventCallback eventCb) = 0;
		virtual void UnubscribeFromStateEvents(StateUnsubscribeToken unsubscribeToken) = 0;

		virtual VaultUnsubscribeToken SubscribeForVaultEvents(VaultEventCallback eventCb) = 0;
		virtual void UnubscribeFromVaultEvents(VaultUnsubscribeToken unsubscribeToken) = 0;

	public:
		virtual ~IScanner() = default;

	protected:
		IScanner() = default;

	private:
		IScanner(const IScanner&) = delete;
		IScanner(IScanner&&) = delete;
		IScanner& operator=(const IScanner&) = delete;
		IScanner& operator=(IScanner&&) = delete;
	};
}
