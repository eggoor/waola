#pragma once

namespace WaolaCli {
	/*
	Keep in sync with Waola::HostField
	*/
	public enum class HostField {
		Undefined = -1
		, DisplayName = 0
		, Hostname
		, IpAddress
		, MacAddress
		, LastSeenOnline
		, OpResult
		, Count
	};
}