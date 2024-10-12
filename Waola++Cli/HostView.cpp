#include "pch.h"

#include "StringExtensions.h"

#include "HostView.h"

using namespace WaolaCli;

HostChangedField HostView::HostFieldToChangedField(HostField hostField)
{
	return (WaolaCli::HostChangedField)Waola::IHostView::HostFieldToChangedField((Waola::HostField)hostField);
}

HostView::HostView(const Waola::IHostView& hostView)
	: hostView(hostView.Clone())
{
}

HostView::~HostView()
{
	if (!disposed) {
		disposed = true;
		this->!HostView();
	}
}

HostView::!HostView()
{
	delete hostView;
}

IntPtr HostView::Id::get()
{
	return IntPtr(hostView->GetHostId());
}

String^ HostView::DisplayName::get()
{
	 return StringExtensions::FromUtf8(hostView->GetDisplayName());
}

String^ HostView::HumanReadableId::get()
{
	 return StringExtensions::FromUtf8(hostView->GetHumanReadableId());
}

String^ HostView::DirectDisplayName::get()
{
	 return StringExtensions::FromUtf8(hostView->GetExactDisplayName());
}

void HostView::DisplayName::set(String^ displayName)
{
	auto nativeDisplayName = StringExtensions::ToUtf8(displayName);
	hostView->SetDisplayName(nativeDisplayName);
	delete[] nativeDisplayName;
}

String^ HostView::Hostname::get()
{
	return StringExtensions::FromUtf8(hostView->GetHostname());
}

void HostView::Hostname::set(String^ hostName)
{
	auto nativeHostname = StringExtensions::ToUtf8(hostName);
	hostView->SetHostname(nativeHostname);
	delete[] nativeHostname;
}

String^ HostView::IpAddressString::get()
{
	return StringExtensions::FromUtf8(hostView->GetIpAddressString());
}

void HostView::IpAddressString::set(String^ ipAddress)
{
	auto nativeIpAddress = StringExtensions::ToUtf8(ipAddress);
	hostView->SetIpAddressString(nativeIpAddress);
	delete[] nativeIpAddress;
}

String^ HostView::MacAddressString::get()
{
	return StringExtensions::FromUtf8(hostView->GetMacAddressString());
}

void HostView::MacAddressString::set(String^ macAddress)
{
	auto nativeMacAddress = StringExtensions::ToUtf8(macAddress);
	hostView->SetMacAddressString(nativeMacAddress);
	delete[] nativeMacAddress;
}

#define ETHER_ADDR_LEN 6

array<System::Byte>^ HostView::MacAddress::get()
{
	const unsigned char* nativeMacAddress = hostView->GetMacAddress();
	array<System::Byte>^ macAddress = gcnew array<System::Byte>(ETHER_ADDR_LEN);

	for (auto i = ETHER_ADDR_LEN; i --> 0;) {
		macAddress[i] = nativeMacAddress[i];
	}

	return macAddress;
}

String^ HostView::LastSeenOnlineString::get()
{
	return StringExtensions::FromUtf8(hostView->GetLastSeenOnlineString());
}

DateTime HostView::LastSeenOnline::get()
{
	auto lso = hostView->GetLastSeenOnline();
	auto offset = DateTimeOffset::FromUnixTimeSeconds(lso);

	return offset > DateTime::UnixEpoch
		? offset.DateTime
		: DateTime::MinValue;
}

HostChangedField HostView::State::get()
{
	return (HostChangedField)hostView->GetHostState();
}

OpResult HostView::Result::get()
{
	return (OpResult)hostView->GetOpResult();
}

Waola::IHostView* HostView::Native::get()
{
	return hostView;
}

void HostView::SetFieldChangeProcessed(HostChangedField hostField)
{
	hostView->SetFieldChangeProcessed((Waola::HostChangedField)hostField);
}

bool WaolaCli::HostView::Equals(Object^ obj)
{
	HostView^ hostView = dynamic_cast<HostView^>(obj);
	return hostView != nullptr ? this->Equals(hostView) : false;
}

bool WaolaCli::HostView::Equals(IHostView^ other)
{
	return Native->Equals(other->Native);
}

int WaolaCli::HostView::GetHashCode()
{
	if (disposed) {
		throw gcnew ObjectDisposedException(gcnew String("WaolaCli::HostView"));
	}

	auto hashCode = HashCode::Combine(DirectDisplayName, Hostname);
	hashCode = HashCode::Combine(hashCode, Hostname);
	hashCode = HashCode::Combine(hashCode, IpAddressString);
	hashCode = HashCode::Combine(hashCode, MacAddressString);

	return hashCode;
}

String^ HostView::ToString()
{
	return StringExtensions::FromUtf8(hostView->ToString());
}
