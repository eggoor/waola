#pragma once

#include "wnet_iface_prov.h"

void wnet_iface_prov_store_ifaces(wnet_iface_prov_t* self,
	int socket, const struct ifconf* ifconf);
void wnet_iface_prov_store_iface(wnet_iface_prov_t* self,
	int socket, const struct ifreq* ifreq);
int wnet_iface_prov_get_iface_index(int socket, const struct ifreq* const ifreq);
byte* ifreq_get_mac_addr(int socket, const struct ifreq* const ifreq);
BOOL wnet_iface_prov_is_suitable_iface_family(sa_family_t saf);
