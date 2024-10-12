#pragma once

#include "include/whost_view_s.h"

#include "whost.h"

struct whost_view
{
	whost_t* host;
	char* ipAddrString;
	char* macAddrString;
	char* lastSeenOnlineString;
	void* extraData;
};

static void update_whost_view_ip_addr(whost_view_t* const restrict hw);
static void update_whost_view_mac_addr(whost_view_t* const restrict hw);
static void update_whost_view_last_seen_online(whost_view_t* const restrict hw, time_t lso);
