#pragma once

#include "wnet_iface_prov_u.h"

static BOOL is_suitable_iface_flags(int socket, const char* const iface_name);
static BOOL is_suitable_iface(int socket, const struct ifreq* const ifreq);
static void ifreq_log(const struct ifreq* const iface);
static void do_add_iface_to_collection(wnet_iface_prov_t* self,
	int socket, const struct ifreq* master_ifreq);
