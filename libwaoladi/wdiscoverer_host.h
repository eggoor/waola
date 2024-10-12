#pragma once

wdiscoverer_iface_t* wdiscoverer_host_get_parent(const wdiscoverer_host_t* self);
int wdiscoverer_host_free(wdiscoverer_host_t* self);
int wdiscoverer_host_begin_discovery(wdiscoverer_host_t* self);
wnet_iface_t* wdiscoverer_host_get_net_iface(const wdiscoverer_host_t* self);
uint32_t wdiscoverer_host_get_target_ip_addr(const wdiscoverer_host_t* self);
byte* wdiscoverer_host_get_mac_addr_buffer(wdiscoverer_host_t* self);
