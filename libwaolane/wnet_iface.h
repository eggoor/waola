#pragma once

#include "include/wnet_iface.h"
#include "include/warper.h"

const waolane_fact4s_t* wnet_iface_get_factories(const wnet_iface_t* self);
int wnet_iface_compare(const wnet_iface_t* self, int ifindex,
	const char* ifname, sa_family_t sa_family);
warper_t* wnet_iface_get_arper(const wnet_iface_t* self);
