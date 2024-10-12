/** @file host_dlg.c
* WaolaG host dialog implementation.
*/

#include "pch.h"

#include "main_wnd.h"

#include "host_dlg.h"
#include "host_dlg_priv.h"

struct _WaolagHostDlg
{
	GtkWindow parent_instance;

	gchar* displayName;
	gchar* hostname;
	gchar* ipAddr;
	gchar* macAddr;

	WaolagMainWnd* main_wnd;
	WaolagHostDlgSaveCb save_cb;
	WaolagHost* host;
};

G_DEFINE_FINAL_TYPE(WaolagHostDlg, waolag_host_dlg, GTK_TYPE_WINDOW)

enum
{
	PROP_0,
	PROP_DISPLAY_NAME,
	PROP_HOSTNAME,
	PROP_IP_ADDR,
	PROP_MAC_ADDR,
	PROP_LAST
};

static void host_dlg_get_property(GObject* object, guint prop_id,
	GValue* value, GParamSpec* properties)
{
	WaolagHostDlg* self =(WaolagHostDlg*)object;

	switch(prop_id)
	{
	case PROP_DISPLAY_NAME:
		g_value_set_string(value, host_dlg_get_display_name(self));
		break;
	case PROP_HOSTNAME:
		g_value_set_string(value, host_dlg_get_hostname(self));
		break;
	case PROP_IP_ADDR:
		g_value_set_string(value, host_dlg_get_ip_addr(self));
		break;
	case PROP_MAC_ADDR:
		g_value_set_string(value, host_dlg_get_mac_addr(self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, properties);
	}
}

static void host_dlg_set_property(GObject* object, guint prop_id,
	const GValue* value, GParamSpec* properties)
{
	WaolagHostDlg* self =(WaolagHostDlg*)object;

	switch(prop_id)
	{
	case PROP_DISPLAY_NAME:
		host_dlg_set_display_name(self, g_value_get_string(value));
		break;
	case PROP_HOSTNAME:
		host_dlg_set_hostname(self, g_value_get_string(value));
		break;
	case PROP_IP_ADDR:
		host_dlg_set_ip_addr(self, g_value_get_string(value));
		break;
	case PROP_MAC_ADDR:
		host_dlg_set_mac_addr(self, g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, properties);
	}
}

static GParamSpec* properties[PROP_LAST];

static void waolag_host_dlg_class_init(WaolagHostDlgClass* klass)
{
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
	G_OBJECT_CLASS(klass)->dispose = waolag_host_dlg_dispose;

	gtk_widget_class_set_template_from_resource(widget_class,
		"/com/asuscomm/smartnclever/WaolaG/host_dlg.ui");
	gtk_widget_class_bind_template_child(widget_class, WaolagHostDlg, displayName);
	gtk_widget_class_bind_template_child(widget_class, WaolagHostDlg, hostname);
	gtk_widget_class_bind_template_child(widget_class, WaolagHostDlg, ipAddr);
	gtk_widget_class_bind_template_child(widget_class, WaolagHostDlg, macAddr);

	GObjectClass* object_class = G_OBJECT_CLASS(klass);

	object_class->get_property = host_dlg_get_property;
	object_class->set_property = host_dlg_set_property;

	properties[PROP_DISPLAY_NAME] = g_param_spec_string("DisplayName", "DisplayName",
		"Host's friendly name", NULL, (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	properties[PROP_HOSTNAME] = g_param_spec_string("Hostname", "Hostname",
		"Host name", NULL, (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	properties[PROP_IP_ADDR] = g_param_spec_string("IpAddress", "IpAddress",
		"IP address", NULL, (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	properties[PROP_MAC_ADDR] = g_param_spec_string("MacAddress", "MacAddress",
		"MAC address", NULL, (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	g_object_class_install_properties(object_class, PROP_LAST, properties);
}

static const GActionEntry dlg_actions[] = {
	{ "save", dlg_action_save },
	{ "cancel", dlg_action_cancel },
};

static void waolag_host_dlg_init(WaolagHostDlg* self)
{
	gtk_widget_init_template(GTK_WIDGET(self));

	GActionGroup* actions = (GActionGroup*)g_simple_action_group_new();
	g_action_map_add_action_entries(G_ACTION_MAP(actions), dlg_actions,
		G_N_ELEMENTS(dlg_actions), self);
	gtk_widget_insert_action_group(GTK_WIDGET(self), "dlg", actions);

	self->displayName = self->hostname = self->ipAddr = self->macAddr = NULL;
	self->host = NULL;
	self->save_cb = NULL;
}

static void waolag_host_dlg_dispose(GObject* gobject)
{
  gtk_widget_dispose_template(GTK_WIDGET(gobject), WAOLAG_TYPE_HOST_DLG);
  G_OBJECT_CLASS(waolag_host_dlg_parent_class)->dispose(gobject);
}

void host_dlg_init(WaolagHostDlg* self, WaolagMainWnd* main_wnd,
	WaolagHostDlgSaveCb cb, WaolagHost* host)
{
	if (host) {
		const gchar* val = NULL;
		GValue gval = G_VALUE_INIT;

		g_value_init (&gval, G_TYPE_STRING);

		val = host_get_exact_display_name(host);
		if (val) {
			g_value_set_string(&gval, val);
			g_object_set_property(G_OBJECT(self), "DisplayName", &gval);
		}

		val = host_get_hostname(host);
		if (val) {
			g_value_set_string(&gval, val);
			g_object_set_property(G_OBJECT(self), "Hostname", &gval);
		}

		val = host_get_ip_addr(host);
		if (val) {
			g_value_set_string(&gval, val);
			g_object_set_property(G_OBJECT(self), "IpAddress", &gval);
		}

		val = host_get_mac_addr(host);
		if (val) {
			g_value_set_string(&gval, val);
			g_object_set_property(G_OBJECT(self), "MacAddress", &gval);
		}

		g_value_unset(&gval);

		self->host = host;
	}

	self->main_wnd = main_wnd;
	self->save_cb = cb;
}

WaolagHost* host_dlg_get_host(WaolagHostDlg* self)
{
	return self->host;
}

static void dlg_action_save(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagHostDlg* self = user_data;
	g_assert(WAOLAG_IS_HOST_DLG(self));

	gtk_window_close(GTK_WINDOW(self));

	if (self->save_cb) {
		(self->save_cb)(self->main_wnd, self);
	}
}

static void dlg_action_cancel(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	GtkWindow* self = user_data;
	g_assert(WAOLAG_IS_HOST_DLG(self));
	gtk_window_close(self);
}

const gchar* host_dlg_get_display_name(WaolagHostDlg* self)
{
	return self->displayName;
}

void host_dlg_set_display_name(WaolagHostDlg* self, const gchar* display_name)
{
	if (g_strcmp0(self->displayName, display_name) != 0) {
		g_free(self->displayName);
		self->displayName = g_strdup(display_name);
	}
}

const gchar* host_dlg_get_hostname(WaolagHostDlg* self)
{
	return self->hostname;
}

void host_dlg_set_hostname(WaolagHostDlg* self, const gchar* hostname)
{
	if (g_strcmp0(self->hostname, hostname) != 0) {
		g_free(self->hostname);
		self->hostname = g_strdup(hostname);
	}
}

const gchar* host_dlg_get_ip_addr(WaolagHostDlg* self)
{
	return self->ipAddr;
}

void host_dlg_set_ip_addr(WaolagHostDlg* self, const gchar* ip_addr)
{
	if (g_strcmp0(self->ipAddr, ip_addr) != 0) {
		g_free(self->ipAddr);
		self->ipAddr = g_strdup(ip_addr);
	}
}

const gchar* host_dlg_get_mac_addr(WaolagHostDlg* self)
{
	return self->macAddr;
}

void host_dlg_set_mac_addr(WaolagHostDlg* self, const gchar* mac_addr)
{
	if (g_strcmp0(self->macAddr, mac_addr) != 0) {
		g_free(self->macAddr);
		self->macAddr = g_strdup(mac_addr);
	}
}
