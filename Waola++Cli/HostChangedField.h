#pragma once

namespace WaolaCli {
	/*
	Keep in sync with Waola::HostChangedField
	*/
	public enum class HostChangedField {
		Unchanged = 0x00
		, DisplayName = 0x01
		, Hostname = 0x02
		, IpAddress = 0x04
		, MacAddress = 0x08
		, LastSeenOnline = 0x10
		, OpResult = 0x20
		, All = DisplayName | Hostname | IpAddress | MacAddress | LastSeenOnline | OpResult
	};
}
