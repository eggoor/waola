#include "pch.h"

#include "HostData.h"

using namespace WaolaCli;

HostData::HostData()
	: HostData(String::Empty, String::Empty, String::Empty,
		String::Empty, DateTime::MinValue)
{
}

HostData::HostData(String^ displayName, String^ hostName,
	String^ ipAddr, String^ macAddr,
	DateTime lastSeenOnline)
{
	DisplayName = displayName;
	Hostname = hostName;
	IpAddr = ipAddr;
	MacAddr = macAddr;
	LastSeenOnline = lastSeenOnline;
}

String^ HostData::DisplayName::get()
{
	return displayName;
}

void HostData::DisplayName::set(String^ displayName)
{
	this->displayName = displayName;
}

String^ HostData::Hostname::get()
{
	return hostName;
}

void HostData::Hostname::set(String^ hostName)
{
	this->hostName = hostName;
}

String^ HostData::IpAddr::get()
{
	return ipAddr;
}

void HostData::IpAddr::set(String^ ipAddr)
{
	this->ipAddr = ipAddr;
}

String^ HostData::MacAddr::get()
{
	return macAddr;
}

void HostData::MacAddr::set(String^ macAddr)
{
	this->macAddr = macAddr;
}

DateTime HostData::LastSeenOnline::get()
{
	return lastSeenOnline;
}

void HostData::LastSeenOnline::set(DateTime lastSeenOnline)
{
	this->lastSeenOnline = lastSeenOnline;
}
