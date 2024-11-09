#pragma once

namespace Waola {
	/*
	Keep in sync with whost_changed_field_t
	*/
	enum HostChangedField {
		cf_unchanged			= 0x00
		, cf_display_name		= 0x01
		, cf_host_name			= 0x02
		, cf_ip_address			= 0x04
		, cf_mac_address		= 0x08
		, cf_last_seen_online	= 0x10
		, cf_op_result			= 0x20
		, cf_all = cf_display_name | cf_host_name | cf_ip_address | cf_mac_address | cf_last_seen_online | cf_op_result
	};
}
