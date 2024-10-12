#include "pch.h"

#include "HostView.h"

using namespace Waola;

HostChangedField IHostView::HostFieldToChangedField(HostField hf)
{
	return HostChangedField(whost_field_2_changed_field((whost_field_t)hf));
}

HostView::HostView(whost_view_t* const host_view)
	: hostView(host_view)
{
}

const HostId HostView::GetHostId() const
{
	return hostView;
}

std::string HostView::GetHumanReadableId() const
{
	auto displayName = whost_view_get_host_id(hostView);
	return displayName ? displayName : std::string();
}

std::string HostView::GetDisplayName() const
{
	auto displayName = whost_view_get_display_name(hostView);
	return displayName ? displayName : std::string();
}

std::string HostView::GetExactDisplayName() const
{
	auto displayName = whost_view_get_exact_display_name(hostView);
	return displayName ? displayName : std::string();
}

void HostView::SetDisplayName(const char* const displayName)
{
	whost_view_set_display_name(hostView, displayName);
}

std::string HostView::GetHostname() const
{
	auto hostName = whost_view_get_hostname(hostView);
	return hostName ? hostName : std::string();
}

void HostView::SetHostname(const char* const hostName)
{
	whost_view_set_hostname(hostView, hostName);
}

std::string HostView::GetIpAddressString() const
{
	auto ipAddress = whost_view_get_ip_addr(hostView);
	return ipAddress ? ipAddress : std::string();
}

void HostView::SetIpAddressString(const char* const ipAddress)
{
	whost_view_set_ip_addr(hostView, ipAddress);
}

std::string HostView::GetMacAddressString() const
{
	auto macAddress = whost_view_get_mac_addr(hostView);
	return macAddress ? macAddress : std::string();
}

void HostView::SetMacAddressString(const char* const macAddressString)
{
	whost_view_set_mac_addr_string(hostView, macAddressString);
}

std::string HostView::GetLastSeenOnlineString() const
{
	auto lastSeenOnline = whost_view_get_last_seen_online(hostView);
	return lastSeenOnline ? lastSeenOnline : std::string();
}

const unsigned char* HostView::GetMacAddress() const
{
	auto host = whost_view_get_host(hostView);
	return whost_get_mac_addr(host);
}

time_t HostView::GetLastSeenOnline() const
{
	auto host = whost_view_get_host(hostView);
	return whost_get_last_seen_online(host);
}

OpResult HostView::GetOpResult() const
{
	auto host = whost_view_get_host(hostView);
	return (OpResult)whost_get_op_result(host);
}

IHostView* HostView::Clone() const
{
	return new HostView(hostView);
}

HostChangedField HostView::GetHostState() const
{
	return (HostChangedField)whost_view_get_state(hostView);
}

void HostView::SetFieldChangeProcessed(HostChangedField changedFields)
{
	whost_view_set_field_change_processed(hostView,
		(whost_changed_field_t)changedFields);
}

std::string HostView::ToString() const
{
	std::stringstream ss;
	bool prevValIsEmpty = true;

	auto str = GetMacAddressString();
	AddToStream(ss, str, prevValIsEmpty);

	str = GetIpAddressString();
	AddToStream(ss, str, prevValIsEmpty);

	str = GetHostname();
	AddToStream(ss, str, prevValIsEmpty);

	str = GetExactDisplayName();
	AddToStream(ss, str, prevValIsEmpty);

	return ss.str();
}

bool HostView::Equals(const IHostView* const other) const
{
	auto otherHostView = dynamic_cast<const HostView*>(other);
	return otherHostView ?
		whost_view_compare(hostView, otherHostView->hostView) == 0
		: false;
}

void HostView::AddToStream(std::stringstream& ss,
	const std::string& str, bool& prevValIsEmpty)
{
	bool isEmpty = str.empty();

	if (!isEmpty)
	{
		if (!prevValIsEmpty)
		{
			ss << '\t';
		}

		ss << str;
	}
	
	prevValIsEmpty = isEmpty;
}
