#pragma once

#include "whost.h"

struct whost {
	char* displayName;
	char* hostname;
	time_t lastSeenOnline;
	struct sockaddr* sockAddr;
	whost_changed_field_t fieldState;
	whost_op_result_t opResult;
	byte macAddr[ETHER_ADDR_LEN];
};


static void whost_do_set_sockaddr(whost_t* host, const struct sockaddr* const sa);
static BOOL whost_needs_set_display_name(whost_t* host, const char* const displayName);
static BOOL is_better_hostname(const whost_t* host, const char* const hostName);
