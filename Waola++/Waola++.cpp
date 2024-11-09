#include "pch.h"

#include "Waola++.h"

Waola::IWaola* Waola::IWaola::Create()
{
	return new Waola();
}

void Waola::IWaola::Destroy(Waola::IWaola* const waola)
{
	delete waola;
}

void Waola::Waola::WakeUp(std::vector<IHostView*> hostList)
{
	std::vector<whost_view_t*> hosts(hostList.size());
	std::transform(cbegin(hostList), cend(hostList), begin(hosts),
		[](IHostView* const hostView) {
			return reinterpret_cast<whost_view_t*>(hostView->GetHostId());
		});

	int rc = waolah(hosts.data(), hosts.size());

	if (rc) {
		std::ostringstream os;
		os << "Sending magic packet finished with error code " << rc;
		throw std::runtime_error(os.str());
	}
}
