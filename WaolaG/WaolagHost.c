#include "pch.h"

#include "waolag.h"

#include "WaolagHost.h"

struct _WaolagHost
{
	GObject parent_instance;

	whost_view_t* host_view;
};

G_DEFINE_FINAL_TYPE(WaolagHost, waolag_host, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_DISPLAY_NAME,
	PROP_HOSTNAME,
	PROP_IP_ADDR,
	PROP_MAC_ADDR,
	PROP_LSO,
	PROP_OP_RESULT,
	PROP_LAST
};

static void host_get_property(GObject* object, guint prop_id,
								GValue* value, GParamSpec* properties)
{
	WaolagHost* self =(WaolagHost*)object;

	switch(prop_id)
	{
	case PROP_DISPLAY_NAME:
		g_value_set_string(value, host_get_display_name(self));
		break;
	case PROP_HOSTNAME:
		g_value_set_string(value, host_get_hostname(self));
		break;
	case PROP_IP_ADDR:
		g_value_set_string(value, host_get_ip_addr(self));
		break;
	case PROP_MAC_ADDR:
		g_value_set_string(value, host_get_mac_addr(self));
		break;
	case PROP_LSO:
		g_value_set_string(value, host_get_lso(self));
		break;
	case PROP_OP_RESULT:
		g_value_set_string(value, host_get_op_result(self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, properties);
	}
}

static void host_set_property(GObject* object, guint prop_id,
								const GValue* value, GParamSpec* properties)
{
	WaolagHost* self =(WaolagHost*)object;

	switch(prop_id)
	{
	case PROP_DISPLAY_NAME:
		host_set_display_name(self, g_value_get_string(value));
		break;
	case PROP_HOSTNAME:
		host_set_hostname(self, g_value_get_string(value));
		break;
	case PROP_IP_ADDR:
		host_set_ip_addr(self, g_value_get_string(value));
		break;
	case PROP_MAC_ADDR:
		host_set_mac_addr(self, g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, properties);
	}
}

static GParamSpec* properties[PROP_LAST];

static void waolag_host_class_init(WaolagHostClass* klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS(klass);

	object_class->get_property = host_get_property;
	object_class->set_property = host_set_property;

	properties[PROP_DISPLAY_NAME] = g_param_spec_string("DisplayName", "DisplayName",
		"Host's friendly name", NULL, (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	properties[PROP_HOSTNAME] = g_param_spec_string("Hostname", "Hostname",
		"Host name", NULL, (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	properties[PROP_IP_ADDR] = g_param_spec_string("IpAddress", "IpAddress",
		"IP address", NULL, (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	properties[PROP_MAC_ADDR] = g_param_spec_string("MacAddress", "MacAddress",
		"MAC address", NULL, (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	properties[PROP_LSO] = g_param_spec_string("LastSeenOnline", "LastSeenOnline",
		"Last seen onlie", NULL, (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

	properties[PROP_OP_RESULT] = g_param_spec_string("WakeupResult", "WakeupResult",
		"Wakeup result", NULL, (G_PARAM_READABLE | G_PARAM_STATIC_STRINGS));

	g_object_class_install_properties(object_class, PROP_LAST, properties);
}

static void waolag_host_init(WaolagHost* self)
{
	self->host_view = NULL;
}

WaolagHost* waolag_host_new(whost_view_t* hw)
{
	WaolagHost* host = g_object_new(WAOLAG_HOST_TYPE, NULL);
	host->host_view = hw;
	wlog_if_level(wll_debug,"%s: %s: hw == 0x%p, host == 0x%p\n",
		__func__, whost_view_get_host_id(hw), (void*)hw, (void*)host);
	whost_view_set_extra_data(hw, &host, sizeof host);
	return host;
}

const gchar* host_get_display_name(WaolagHost* self)
{
	return whost_view_get_display_name(self->host_view);
}

void host_set_display_name(WaolagHost* self, const gchar* display_name)
{
	whost_view_set_display_name(self->host_view, display_name);
}

const gchar* host_get_hostname(WaolagHost* self)
{
	return whost_view_get_hostname(self->host_view);
}

void host_set_hostname(WaolagHost* self, const gchar* hostname)
{
	whost_view_set_hostname(self->host_view, hostname);
}

const gchar* host_get_ip_addr(WaolagHost* self)
{
	return whost_view_get_ip_addr(self->host_view);
}

void host_set_ip_addr(WaolagHost* self, const gchar* ip_addr)
{
	whost_view_set_ip_addr(self->host_view, ip_addr);
}

const gchar* host_get_mac_addr(WaolagHost* self)
{
	return whost_view_get_mac_addr(self->host_view);
}

void host_set_mac_addr(WaolagHost* self, const gchar* mac_addr)
{
	whost_view_set_mac_addr_string(self->host_view, mac_addr);
}

const gchar* host_get_lso(WaolagHost* self)
{
	return whost_view_get_last_seen_online(self->host_view);
}

const gchar* host_get_op_result(WaolagHost* self)
{
	return whost_view_get_op_result(self->host_view);
}

const gchar* host_get_exact_display_name(WaolagHost* self)
{
	return whost_view_get_exact_display_name(self->host_view);
}

whost_view_t* host_get_host_view(WaolagHost* self)
{
	return self->host_view;
}

void host_notify_property_changed(WaolagHost* self)
{
	whost_view_t* hw = host_get_host_view(self);

	if (whost_view_is_field_updated(hw, hwf_display_name)) {
		g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_DISPLAY_NAME]);
		whost_view_set_field_change_processed(hw, cf_display_name);
	}

	if (whost_view_is_field_updated(hw, hwf_host_name)) {
		g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_HOSTNAME]);
		whost_view_set_field_change_processed(hw, cf_host_name);
	}

	if (whost_view_is_field_updated(hw, hwf_ip_address)) {
		g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_IP_ADDR]);
		whost_view_set_field_change_processed(hw, cf_ip_address);
	}

	if (whost_view_is_field_updated(hw, hwf_mac_address)) {
		g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_MAC_ADDR]);
		whost_view_set_field_change_processed(hw, cf_mac_address);
	}

	if (whost_view_is_field_updated(hw, hwf_last_seen_online)) {
		g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_LSO]);
		whost_view_set_field_change_processed(hw, cf_last_seen_online);
	}

	if (whost_view_is_field_updated(hw, hwf_op_result)) {
		g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_OP_RESULT]);
		whost_view_set_field_change_processed(hw, cf_op_result);
	}
}
