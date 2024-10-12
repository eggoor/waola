#pragma once

namespace Waola {
	/*
	Keep in sync with whost_field_t
	*/
	enum HostField {
		hwf_undefined = -1
		, hwf_display_name = 0
		, hwf_host_name
		, hwf_ip_address
		, hwf_mac_address
		, hwf_last_seen_online
		, hwf_op_result
		, hwf_count
	};
}