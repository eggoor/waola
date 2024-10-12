/** @file main_wnd_priv.h
* WaolaG main window private declarations.
*/
#pragma once

#include "waola_event_data.h"

#include "WaolagHost.h"

static void wnd_action_wakeup(GSimpleAction* action, GVariant* parameter, gpointer user_data);
static void wnd_action_scan(GSimpleAction* action, GVariant* parameter, gpointer user_data);
static void wnd_action_refresh(GSimpleAction* action, GVariant* parameter, gpointer user_data);
static void wnd_action_add(GSimpleAction* action, GVariant* parameter, gpointer user_data);
static void wnd_action_edit(GSimpleAction* action, GVariant* parameter, gpointer user_data);
static void wnd_action_copy(GSimpleAction* action, GVariant* parameter, gpointer user_data);
static void wnd_action_delete(GSimpleAction* action, GVariant* parameter, gpointer user_data);
static void show_help_overlay(GSimpleAction* action, GVariant* parameter, gpointer user_data);

static void on_destroy(GtkWidget* widget, gpointer user_data);
static void on_waolasc_state_event_threaded(const waolasc_cbi_t* cbi);
static gboolean on_waolasc_state_event(gpointer data);
static void on_vault_event_threaded(const waolava_cbi_t* cbi);
static gboolean on_vault_event(gpointer data);
static void on_vault_event_host_added(const waola_event_data_t* wed);
static void on_vault_event_host_modified(const waola_event_data_t* wed);
static void on_vault_event_host_deleted(const waola_event_data_t* wed);
static GSettings* get_settings(GSettingsSchemaSource* settings_schema_source);
static GArray* load_hosts(GSettingsSchemaSource* settings_schema_source);
static void save_hosts(GSettingsSchemaSource* settings_schema_source, const wcont4r_t* hosts);
static GSettingsSchemaSource* get_settings_schema_source(void);
static void bind_settings(WaolagMainWnd* self);

static void scan_async(WaolagMainWnd* self);
static gpointer scan_thread_func(gpointer data);

static void refresh_async(WaolagMainWnd* self);
static gpointer refresh_thread_func(gpointer data);

static GPtrArray* get_selected_host_views(WaolagMainWnd* self);
static WaolagHost* get_selected_host(WaolagMainWnd* self);

static void on_host_dlg_save(WaolagMainWnd* self, WaolagHostDlg* dlg);
static void on_row_activated(G_GNUC_UNUSED GtkColumnView* column_view,
	guint position, gpointer data);
	
static void edit_selected_host(WaolagMainWnd* self);
static void wakeup_selected_hosts(WaolagMainWnd* self);