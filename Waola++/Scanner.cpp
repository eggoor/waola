/** @file Scanner.c
* C++ binding to Waola scanner implementation.
*/

#include "pch.h"

#include "HostView.h"

#include "Scanner.h"

using namespace Waola;

IScanner* IScanner::Create() {
	return new Scanner();
}

void IScanner::Destroy(IScanner* const scanner) {
	delete scanner;
}

Scanner::Scanner()
	: stateCbi {
		.caller = this,
		.cb = &Scanner::StateCallback
	}
	, vaultCbi {
		.caller = this,
		.cb = &Scanner::VaultCallback
	}
	, scanner(wmake_waolasc_def(&stateCbi, &vaultCbi))
{
}

Scanner::~Scanner()
{
	waolasc_unsubscribe_state(scanner, &stateCbi);
	waolasc_unsubscribe_vault(scanner, &vaultCbi);
	waolasc_free(scanner);
}

void Scanner::StateCallback(const waolasc_cbi_t* const cbi)
{
	((Scanner*)(cbi->caller))->StateCallback(cbi->tasks);
}

void Scanner::VaultCallback(const waolava_cbi_t* const cbi)
{
	((Scanner*)(cbi->caller))->VaultCallback(cbi->opCode,
		cbi->hostView);
}

void Scanner::StateCallback(wtask_t tasks)
{
	stateObserver.Notify(StateEvent((Task)tasks));
}

void Scanner::VaultCallback(waolava_op_t opCode,
	whost_view_t* host_view)
{
	HostView hostView(host_view);
	VaultEvent vaultEvent((VaultOperation)opCode, hostView);

	vaultObserver.Notify(vaultEvent);
}

VaultUnsubscribeToken Scanner::SubscribeForVaultEvents(VaultEventCallback eventCb)
{
	return vaultObserver.Subscribe(eventCb);
}

void Scanner::UnubscribeFromVaultEvents(VaultUnsubscribeToken unsubscribeToken)
{
	vaultObserver.Unubscribe(unsubscribeToken);
}

StateUnsubscribeToken Scanner::SubscribeForStateEvents(StateEventCallback eventCb)
{
	return stateObserver.Subscribe(eventCb);
}

void Scanner::UnubscribeFromStateEvents(StateUnsubscribeToken unsubscribeToken)
{
	stateObserver.Unubscribe(unsubscribeToken);
}

bool Scanner::IsVaultEmpty() const
{
	return waolasc_is_empty(scanner);
}

Task Scanner::GetCurrentTasks() const
{
	return (Task)waolasc_get_current_tasks(scanner);
}

std::forward_list<IHostView*> Scanner::GetHosts()
{
	std::forward_list<IHostView*> hostList;
	auto beforeEnd = hostList.before_begin();

	auto host_list = waolasc_get_host_list(scanner);
	auto host_view = wcont4r_get_first(host_list);
	
	while (host_view) {
		hostList.emplace_after(beforeEnd,
			new HostView(reinterpret_cast<whost_view_t*>(host_view)));
		++beforeEnd;
		host_view = wcont4r_get_next(host_list);
	}

	return hostList;
}

void Scanner::SetLogLevel(LogLevel logLevel)
{
	waolasc_set_log_level((wlog_level_t)logLevel);
}

void Scanner::SetMaxThreads(uint32_t maxThreads)
{
	waolasc_set_max_threads(scanner, maxThreads);
}

std::future<void> Scanner::Discover()
{
	return std::async([this]() {
		int rc = waolasc_discover(scanner);
		if (rc) {
			std::ostringstream os;
			os << "Discovery finished with error code " << rc;
			throw std::runtime_error(os.str());
		}
	});
}

void Scanner::DiscoverAsync()
{
	int rc = waolasc_discover_async(scanner);
	if (rc) {
		std::ostringstream os;
		os << "Asynchronous discovery finished with error code " << rc;
		throw std::runtime_error(os.str());
	}
}

std::future<void> Scanner::Refresh()
{
	return std::async([this]() {
		int rc = waolasc_refresh(scanner);
		if (rc) {
			std::ostringstream os;
			os << "Resresh finished with error code " << rc;
			throw std::runtime_error(os.str());
		}
	});
}

void Scanner::RefreshHost(IHostView* const hostView)
{
	waolasc_refresh_host(scanner,
		reinterpret_cast<whost_view_t*>(hostView->GetHostId()));
}

std::future<void> Scanner::Add(const HostData& hostData)
{
	whost_data_t* host_data = whost_data_make(hostData.DisplayName,
		hostData.Hostname, hostData.IpAddr, hostData.MacAddr,
		hostData.LastSeenOnline);

	auto future = std::async([this, host_data]() {
		waolasc_add(scanner, host_data);
		whost_data_free(host_data);
	});
	
	return future;
}

void Scanner::BatchAdd(std::vector<const HostData*> hostDataList)
{
	std::vector<whost_data_t*> hosts(hostDataList.size());
	size_t i = 0;

	for (const auto& hostData : hostDataList) {
		whost_data_t* host_data = whost_data_make(hostData->DisplayName,
			hostData->Hostname, hostData->IpAddr, hostData->MacAddr,
			hostData->LastSeenOnline);
		hosts[i++] = host_data;
	}

	waolasc_batch_add(scanner, hosts.data(), hosts.size());
}

void Scanner::DeleteHost(IHostView* const hostView)
{
	waolasc_delete_host(scanner,
		reinterpret_cast<whost_view_t*>(hostView->GetHostId()));
}

void Scanner::CancelTask()
{
	waolasc_cancel_task(scanner);
}

const void* Scanner::GetStatus(Module* module, int* const opCode)
{
	return waolasc_get_status(scanner, (wmodule_t*)module, opCode);
}
