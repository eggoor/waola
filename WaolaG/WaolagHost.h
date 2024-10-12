#pragma once

#define WAOLAG_HOST_TYPE (waolag_host_get_type())
G_DECLARE_FINAL_TYPE(WaolagHost, waolag_host, WAOLAG, HOST, GObject)

WaolagHost* waolag_host_new(whost_view_t* hw);

const gchar* host_get_display_name(WaolagHost* self);
void host_set_display_name(WaolagHost* self, const gchar* name);

const gchar* host_get_hostname(WaolagHost* self);
void host_set_hostname(WaolagHost* self, const gchar* name);

const gchar* host_get_ip_addr(WaolagHost* self);
void host_set_ip_addr(WaolagHost* self, const gchar* name);

const gchar* host_get_mac_addr(WaolagHost* self);
void host_set_mac_addr(WaolagHost* self, const gchar* name);

const gchar* host_get_lso(WaolagHost* self);
const gchar* host_get_op_result(WaolagHost* self);
const gchar* host_get_exact_display_name(WaolagHost* self);
whost_view_t* host_get_host_view(WaolagHost* self);

void host_notify_property_changed(WaolagHost* self);
