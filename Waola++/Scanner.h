#pragma once

#include "include/IScanner.h"

#include "Observer.h"

namespace Waola {

	class Scanner : public IScanner
	{
	public:
		Scanner();
		~Scanner();

	public:
		// IScanner implementation 
		bool IsVaultEmpty() const override;
		Task GetCurrentTasks() const override;
		std::forward_list<IHostView*> GetHosts() override;
		void SetLogLevel(LogLevel logLevel) override;
		void SetMaxThreads(uint32_t maxThreads) override;
		std::future<void> Discover() override;
		void DiscoverAsync() override;
		std::future<void> Refresh() override;
		void RefreshHost(IHostView* const hostView) override;
		std::future<void> Add(const HostData& hostData) override;
		void BatchAdd(std::vector<const HostData*> hostDataList) override;
		void DeleteHost(IHostView* const hostView) override;
		void CancelTask() override;
		const void* GetStatus(Module* module, int* const opCode) override;

		StateUnsubscribeToken SubscribeForStateEvents(const StateEventCallbackData& cbData) override;
		void UnsubscribeFromStateEvents(StateUnsubscribeToken unsubscribeToken) override;

		VaultUnsubscribeToken SubscribeForVaultEvents(const VaultEventCallbackData& cbData) override;
		void UnsubscribeFromVaultEvents(VaultUnsubscribeToken unsubscribeToken) override;

	private:
		static void StateCallback(const waolasc_cbi_t* const cbi);
		static void VaultCallback(const waolava_cbi_t* const cbi);
		
		void StateCallback(wtask_t tasks);
		void VaultCallback(waolava_op_t opCode, whost_view_t* const host_view);

	private:
		waolasc_subscribe_data_t stateCbi;
		waolava_subscribe_data_t vaultCbi;
		waolasc_t* scanner;

		Observer<VaultEventCallbackData, VaultUnsubscribeToken, VaultEvent> vaultObserver;
		Observer<StateEventCallbackData, StateUnsubscribeToken, StateEvent> stateObserver;
		
	private:
		Scanner(const IScanner&) = delete;
		Scanner(IScanner&&) = delete;
		Scanner& operator=(const IScanner&) = delete;
		Scanner& operator=(IScanner&&) = delete;
	};
}
