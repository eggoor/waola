/** @file host_dlg_priv.h
* WaolaG host dialog private declarations.
*/
#pragma once

static void waolag_host_dlg_dispose(GObject* gobject);

static void host_dlg_get_property(GObject* object, guint prop_id,
	GValue* value, GParamSpec* properties);
static void host_dlg_set_property(GObject* object, guint prop_id,
	const GValue* value, GParamSpec* properties);

static void dlg_action_save(GSimpleAction* action, GVariant* parameter, gpointer user_data);
static void dlg_action_cancel(GSimpleAction* action, GVariant* parameter, gpointer user_data);
