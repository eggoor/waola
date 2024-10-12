/** @file main_wnd.c
* WaolaG main window implementation.
*/

#include "pch.h"

#include "main_wnd.h"

#include "host_dlg.h"

#include "main_wnd_priv.h"

struct _WaolagMainWnd
{
	GtkApplicationWindow parent_instance;

	/* Template widgets */
	GtkHeaderBar* headerBar;
	GtkBox* leftBar;
	GtkColumnView* columnView;
	GListStore* hostStore;
	waolasc_t* waolasc;
	waolasc_subscribe_data_t state_cbi;
	waolava_subscribe_data_t vault_cbi;
	GThread* task_thread;
	GdkCursor* progress_cursor;
	GdkCursor* busy_cursor;
	GtkWindow* host_dialog;
	guint32 host_counter;
	GSettingsSchemaSource* settings_schema_source;
};

G_DEFINE_FINAL_TYPE(WaolagMainWnd, waolag_main_wnd, GTK_TYPE_APPLICATION_WINDOW)

static void waolag_main_wnd_class_init(WaolagMainWndClass* klass)
{
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
	gtk_widget_class_set_template_from_resource(widget_class,
		"/com/asuscomm/smartnclever/WaolaG/main_wnd.ui");
	gtk_widget_class_bind_template_child(widget_class, WaolagMainWnd, headerBar);
	gtk_widget_class_bind_template_child(widget_class, WaolagMainWnd, leftBar);
	gtk_widget_class_bind_template_child(widget_class, WaolagMainWnd, hostStore);
	gtk_widget_class_bind_template_child(widget_class, WaolagMainWnd, columnView);

	gtk_widget_class_bind_template_callback(widget_class, on_row_activated);
}

static const GActionEntry wnd_actions[] = {
	{ "wakeup", wnd_action_wakeup },
	{ "scan", wnd_action_scan},
	{ "refresh", wnd_action_refresh },
	{ "add", wnd_action_add },
	{ "edit", wnd_action_edit },
	{ "copy", wnd_action_copy },
	{ "delete", wnd_action_delete },
};

static void waolag_main_wnd_init(WaolagMainWnd* self)
{
	g_signal_connect(self, "destroy", G_CALLBACK(on_destroy), NULL);

	gtk_widget_init_template(GTK_WIDGET(self));
	g_action_map_add_action_entries(G_ACTION_MAP(self),
		wnd_actions, G_N_ELEMENTS(wnd_actions), self);

	self->state_cbi.caller = self;
	self->state_cbi.cb = on_waolasc_state_event_threaded;

	self->vault_cbi.caller = self;
	self->vault_cbi.cb = on_vault_event_threaded;

	self->waolasc = wmake_waolasc_def(&self->state_cbi,
		 &self->vault_cbi);
	
	if (!self->waolasc) {
		wlog_if_level(wll_fatal, "Unable to make waola controller\n");
		exit(EX_OSERR);
	}

	self->task_thread = NULL;
	self->progress_cursor = gdk_cursor_new_from_name("progress", NULL);
	self->busy_cursor = gdk_cursor_new_from_name("wait", NULL);
	self->host_dialog = NULL;

	self->host_counter = 0;

	self->settings_schema_source = get_settings_schema_source();

	if (self->settings_schema_source) {
		bind_settings(self);
	}
	else {
		wlog_if_level(wll_warning, "%s: Couldn't get settings schema source\n", __func__);
	}

	if (self->settings_schema_source) {
		GArray* hosts = load_hosts(self->settings_schema_source);

		if (hosts && hosts->len > 0) {
			waolasc_batch_add(self->waolasc, (whost_data_t**)hosts->data, hosts->len);
			g_array_free(hosts, TRUE);
		}
		else {
			scan_async(self);
		}
	}
}

static void on_destroy(GtkWidget* widget, gpointer user_data)
{
	WaolagMainWnd* self = (WaolagMainWnd*)widget;
	g_assert(WAOLAG_IS_MAIN_WND(self));

	waolasc_unsubscribe_vault(self->waolasc, &self->vault_cbi);
	waolasc_unsubscribe_state(self->waolasc, &self->state_cbi);

	if (waolasc_get_current_tasks(self->waolasc)) {
		waolasc_cancel_task(self->waolasc);
		g_free(self->task_thread);
	}

	if (self->settings_schema_source) {
		const wcont4r_t* hosts = waolasc_get_host_list(self->waolasc);
		save_hosts(self->settings_schema_source, hosts);
		g_free(self->settings_schema_source);
	}

	waolasc_free(self->waolasc);
	g_free(self->busy_cursor);
	g_free(self->progress_cursor);
}

static void bind_settings(WaolagMainWnd* self)
{
	GSettings* settings = get_settings(self->settings_schema_source);

	if (settings) {
		g_settings_bind(settings, "width", self,
			"default-width", G_SETTINGS_BIND_DEFAULT);
		g_settings_bind(settings, "height", self,
			"default-height", G_SETTINGS_BIND_DEFAULT);
		g_settings_bind(settings, "is-maximized",
			self, "maximized", G_SETTINGS_BIND_DEFAULT);
		g_settings_bind(settings, "is-fullscreen",
			self, "fullscreened", G_SETTINGS_BIND_DEFAULT);
		g_object_unref(settings);
	}
	else {
		wlog_if_level(wll_warning, "%s: Couldn't get settings\n", __func__);
	}
}

static void wnd_action_wakeup(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagMainWnd* self = user_data;
	g_assert(WAOLAG_IS_MAIN_WND(self));
	
	wakeup_selected_hosts(self);
}

static void wnd_action_scan(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagMainWnd* self = user_data;
	g_assert(WAOLAG_IS_MAIN_WND(self));
	scan_async(self);
}

static void wnd_action_refresh(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagMainWnd* self = user_data;
	g_assert(WAOLAG_IS_MAIN_WND(self));
	refresh_async(self);
}

static void wnd_action_add(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagMainWnd* self = user_data;
	g_assert(WAOLAG_IS_MAIN_WND(self));
	
	self->host_dialog = g_object_new(WAOLAG_TYPE_HOST_DLG, "application",
		gtk_window_get_application(GTK_WINDOW(self)), NULL);
	host_dlg_init((WaolagHostDlg*)self->host_dialog, self,
		on_host_dlg_save, NULL);	
	gtk_window_present(self->host_dialog);
	gtk_window_set_transient_for(self->host_dialog, GTK_WINDOW(self));
	gtk_window_set_modal(self->host_dialog, true);
}

static void wnd_action_edit(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagMainWnd* self = user_data;
	g_assert(WAOLAG_IS_MAIN_WND(self));

	edit_selected_host(self);
}

void wakeup_selected_hosts(WaolagMainWnd* self)
{
	GPtrArray* selected_host_views = get_selected_host_views(self);
	waolah((whost_view_t**)selected_host_views->pdata, selected_host_views->len);

	for (guint i = selected_host_views->len; i --> 0;) {
		WaolagHost* host = *(WaolagHost**)whost_view_get_extra_data(selected_host_views->pdata[i]);
		host_notify_property_changed(host);
	}

	g_ptr_array_free(selected_host_views, true);
}

static void edit_selected_host(WaolagMainWnd* self)
{
	WaolagHost* selected_host = get_selected_host(self);

	if (selected_host) {
		self->host_dialog = g_object_new(WAOLAG_TYPE_HOST_DLG, "application",
			gtk_window_get_application(GTK_WINDOW(self)), NULL);
		host_dlg_init((WaolagHostDlg*)self->host_dialog, self,
			on_host_dlg_save, selected_host);	
		gtk_window_present(self->host_dialog);
		gtk_window_set_transient_for(self->host_dialog, GTK_WINDOW(self));
		gtk_window_set_modal(self->host_dialog, true);
	}
}

static void wnd_action_copy(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagMainWnd* self = user_data;
	g_assert(WAOLAG_IS_MAIN_WND(self));
	
	char* text = NULL;

	GPtrArray* selected_hosts = get_selected_host_views(self);
	
	for (guint i = 0; i < selected_hosts->len;) {
		whost_view_t* host_view = g_ptr_array_index(selected_hosts, i);
		char* host_string = whost_view_to_string(host_view);

		++i;

		if (host_string) {
			text = wstrcat(text, host_string);
			wfree(host_string);

			if (i < selected_hosts->len) {
				text = wstrcat(text, "\n");
			}
		}
	}

	g_ptr_array_free(selected_hosts, true);

	if (text) {
		GdkDisplay* display = gdk_display_get_default();
		GdkClipboard* clipboard = gdk_display_get_clipboard(display);
		gdk_clipboard_set_text(clipboard, text);
		wfree(text);
	}
}

static void wnd_action_delete(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagMainWnd* self = user_data;
	g_assert(WAOLAG_IS_MAIN_WND(self));

	GPtrArray* selected_hosts = get_selected_host_views(self);
	
	for (guint i= selected_hosts->len; i --> 0;) {
		whost_view_t* host_view = selected_hosts->pdata[i];
		waolasc_delete_host(self->waolasc, host_view);
	}

	g_ptr_array_free(selected_hosts, true);
}

static void on_row_activated(G_GNUC_UNUSED GtkColumnView* column_view,
	guint position, gpointer data)
{
	WaolagMainWnd* self = data;
	g_assert(WAOLAG_IS_MAIN_WND(self));

	wakeup_selected_hosts(self);
}

static void show_help_overlay(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	WaolagMainWnd* self = user_data;
	g_assert(WAOLAG_IS_MAIN_WND(self));
}

static void on_host_dlg_save(WaolagMainWnd* self, WaolagHostDlg* dlg)
{	
	gtk_widget_set_cursor(GTK_WIDGET(self), self->busy_cursor);
	
	const gchar* displayName = host_dlg_get_display_name(dlg);
	const gchar* hostname = host_dlg_get_hostname(dlg);
	const gchar* ipAddr = host_dlg_get_ip_addr(dlg);
	const gchar* macAddr = host_dlg_get_mac_addr(dlg);

	WaolagHost* host = host_dlg_get_host(dlg);
	if (host) {
		host_set_display_name(host, displayName);
		host_set_hostname(host, hostname);
		host_set_ip_addr(host,ipAddr );
		host_set_mac_addr(host, macAddr);
		host_notify_property_changed(host);
	}
	else {
		whost_data_t* hd = whost_data_make(displayName, hostname, ipAddr,
			macAddr, 0);
		waolasc_add(self->waolasc, hd);
		whost_data_free(hd);
	}

	gtk_widget_set_cursor(GTK_WIDGET(self), NULL);
}

static GPtrArray* get_selected_host_views(WaolagMainWnd* self)
{
	GtkSelectionModel* model = gtk_column_view_get_model(self->columnView);
	GtkBitset* bitset = gtk_selection_model_get_selection(model);

	GPtrArray* selected_hosts = g_ptr_array_new();

	for (guint64 i = gtk_bitset_get_size(bitset); i --> 0;) {
		guint x = gtk_bitset_get_nth(bitset, i);
		WaolagHost* selected_host = g_list_model_get_item((GListModel*)model, x);
		whost_view_t* host_view = host_get_host_view(selected_host);
		g_ptr_array_add(selected_hosts, host_view);
	}
	gtk_bitset_unref(bitset);

	return selected_hosts;
}

static WaolagHost* get_selected_host(WaolagMainWnd* self)
{
	WaolagHost* selected_host = NULL;
	GtkSelectionModel* selection_model = gtk_column_view_get_model(self->columnView);
	GListModel* model = (GListModel*)selection_model;

	for (guint i = 0; i < g_list_model_get_n_items(model); ++i) {
		if (gtk_selection_model_is_selected(selection_model, i)) {
			selected_host = g_list_model_get_item(model, i);
			break;
		}
	}

	return selected_host;
}

static void scan_async(WaolagMainWnd* self)
{
	if (self->task_thread) {
		wlog_if_level(wll_verbose, "Cancelling task\n");
		waolasc_cancel_task(self->waolasc);
	}
	else {
		wlog_if_level(wll_verbose, "Starting scan task\n");
		self->task_thread = g_thread_new("scan_async",
			scan_thread_func, self);
	}
}

static gpointer scan_thread_func(gpointer data)
{
	WaolagMainWnd* self = data;
	waolasc_discover_async(self->waolasc);
	return NULL;
}

static void refresh_async(WaolagMainWnd* self)
{
	if (self->task_thread) {
		waolasc_cancel_task(self->waolasc);
	}
	else {
		self->task_thread = g_thread_new("refresh_async",
			refresh_thread_func, self);
	}    
}

static gpointer refresh_thread_func(gpointer data)
{
	WaolagMainWnd* self = data;
	waolasc_refresh(self->waolasc);
	return NULL;
}

static void on_waolasc_state_event_threaded(const waolasc_cbi_t* cbi)
{
	gpointer cbi_copy = g_malloc(sizeof *cbi);
	memcpy(cbi_copy, cbi, sizeof *cbi);
	
	g_idle_add(on_waolasc_state_event, cbi_copy);
}

static gboolean on_waolasc_state_event(gpointer data)
{
	const waolasc_cbi_t* cbi = data;
	WaolagMainWnd* self = (WaolagMainWnd*)cbi->caller;
	g_assert(WAOLAG_IS_MAIN_WND(self));
	wlog_if_level(wll_verbose, "Waola controller state canged to '%d'\n", cbi->tasks);

	if (cbi->tasks) {
		gtk_widget_set_cursor(GTK_WIDGET(self), self->progress_cursor);
	}
	else {
		g_free(self->task_thread);
		self->task_thread = NULL;
		gtk_widget_set_cursor(GTK_WIDGET(self), NULL);
	}

	g_free(data);

	return G_SOURCE_REMOVE;
}

static void on_vault_event_threaded(const waolava_cbi_t* cbi)
{
	waola_event_data_t* wed = g_malloc(sizeof(struct waola_event_data));
	wed->caller = cbi->caller;
	wed->opCode = cbi->opCode;

	if (wva_deleted != cbi->opCode) {
		wed->hostView = cbi->hostView;
	}
	else {
		wed->hostView = NULL;
	}
	
	if (wva_added != cbi->opCode) {
		const void* extra_data = whost_view_get_extra_data(cbi->hostView);
		if (extra_data) {
			wed->host = *(WaolagHost**)extra_data;
		}
		else {
			if (wlog_get_level() >= wll_error &&  wva_added == wva_deleted) {
				wlog(wll_error, "No extradata set for host %s\n",
					whost_view_get_host_id(cbi->hostView));
			}			
			return;
		}		
	}
	else {
		wed->host = NULL;
	}	

	g_idle_add(on_vault_event, wed);
}

static gboolean on_vault_event(gpointer data)
{
	waola_event_data_t* wed = data;
	switch (wed->opCode)
	{
		case wva_added:
			on_vault_event_host_added(wed);
		break;
		case wva_modified:
			on_vault_event_host_modified(wed);
		break;
		case wva_deleted:
			on_vault_event_host_deleted(wed);
		break;
		case wva_undefined:
		default:
			wlog_if_level(wll_warning, "Invalid waolava_op_t value %d passed to %s\n",
				wed->opCode, __func__);
	}

	g_free(data);

	return G_SOURCE_REMOVE;
}

static void on_vault_event_host_added(const waola_event_data_t* wed)
{
	WaolagMainWnd* self = (WaolagMainWnd*)wed->caller;
	g_assert(WAOLAG_IS_MAIN_WND(self));

	WaolagHost* host = waolag_host_new(wed->hostView);
	g_list_store_append(self->hostStore, host);
	whost_view_set_field_change_processed(wed->hostView, cf_all);

	wlog_if_level(wll_verbose, "\t\tHost '%s' stored in host store\n",
		whost_view_get_host_id(wed->hostView));
}

static void on_vault_event_host_modified(const waola_event_data_t* wed)
{
	host_notify_property_changed(wed->host);
}

static void on_vault_event_host_deleted(const waola_event_data_t* wed)
{
	WaolagMainWnd* self = (WaolagMainWnd*)wed->caller;
	g_assert(WAOLAG_IS_MAIN_WND(self));

	guint position;
	if (g_list_store_find(self->hostStore, wed->host, &position)) {
		g_list_store_remove(self->hostStore, position);
	}
	else {
		wlog_if_level(wll_warning, "%s: deleted host not found in the store\n",
			__func__);
	}
	g_object_unref(wed->host);
}

static GArray* load_hosts(GSettingsSchemaSource* settings_schema_source)
{
	GArray* hosts = NULL;

	GSettingsSchema* settings_schema = g_settings_schema_source_lookup(settings_schema_source,
		"com.asuscomm.smartnclever.WaolaG.Hosts", true);

	if (settings_schema)
	{
		GSettings* settings = g_settings_new_full(settings_schema, NULL,
												  "/com/asuscomm/smartnclever/WaolaG/Hosts/");
		if (settings)
		{
			GVariant* value_variant;
			GVariantIter iter;
			value_variant = g_settings_get_value(settings, "hosts");

			if (g_variant_is_of_type(value_variant, G_VARIANT_TYPE("a(sssst)")))
			{
				gchar* displayName = NULL;
				gchar* hostname = NULL;
				gchar* ipAddr = NULL;
				gchar* macAddr = NULL;
				time_t lso;

				hosts = g_array_new(FALSE, FALSE, sizeof(whost_data_t*));;

				g_variant_iter_init(&iter, value_variant);
				while (g_variant_iter_loop(&iter, "(sssst)",
					&displayName, &hostname, &ipAddr, &macAddr, &lso))
				{
					whost_data_t* host_data = whost_data_make(displayName,
						hostname, ipAddr, macAddr, lso);
					g_array_append_val(hosts, host_data);
				}

				g_free(displayName);
				g_free(hostname);
				g_free(ipAddr);
				g_free(macAddr);
			}

			g_variant_unref(value_variant);
			g_object_unref(settings);
		}
		g_free(settings_schema);
	}
	else {
		wlog_if_level(wll_warning, "%s: Couldn't lookup settings schema)\n", __func__);
	}

	return hosts;
}

static void save_hosts(GSettingsSchemaSource* settings_schema_source, const wcont4r_t* hosts)
{
	GSettingsSchema* settings_schema = g_settings_schema_source_lookup(settings_schema_source,
		"com.asuscomm.smartnclever.WaolaG.Hosts", true);

	if (settings_schema)
	{
		GSettings* settings = g_settings_new_full(settings_schema, NULL,
			"/com/asuscomm/smartnclever/WaolaG/Hosts/");
		if (settings)
		{
			GVariantBuilder builder;
			g_variant_builder_init(&builder, G_VARIANT_TYPE ("a(sssst)"));

			whost_view_t* host_view = wcont4r_get_first(hosts);
			while (host_view) {
				const gchar* displayName = whost_view_get_exact_display_name(host_view);
				const gchar* hostname = whost_view_get_hostname(host_view);
				const gchar* ipAddr = whost_view_get_ip_addr(host_view);
				const gchar* macAddr = whost_view_get_mac_addr(host_view);
				const whost_t* host = whost_view_get_host(host_view);
				time_t lso = whost_get_last_seen_online(host);

				g_variant_builder_add(&builder, "(sssst)", displayName ? displayName : "",
					hostname ? hostname : "", ipAddr ? ipAddr : "" , macAddr ? macAddr : "", lso);

					host_view = wcont4r_get_next(hosts);
			}

			GVariant* value_variant = g_variant_builder_end(&builder);;

			g_settings_set_value(settings, "hosts", value_variant);
			g_object_unref(settings);
		}
		g_free(settings_schema);
	}
}

static GSettings* get_settings(GSettingsSchemaSource* settings_schema_source)
{
	GSettings* settings_main = NULL;

	if (settings_schema_source) {
		GSettingsSchema* settings_schema = g_settings_schema_source_lookup(settings_schema_source,
			"com.asuscomm.smartnclever.WaolaG.Window", true);

		if (settings_schema) {
			GSettings* settings = g_settings_new_full(settings_schema, NULL,
				"/com/asuscomm/smartnclever/WaolaG/Window/");

			settings_main = g_settings_get_child(settings, "main");
			g_object_unref(settings);
			g_free(settings_schema);
		}
	}

	return settings_main;
}

static GSettingsSchemaSource* get_settings_schema_source(void)
{
	GSettingsSchemaSource* settings_schema_source = NULL;

#ifdef DEBUG
	const char* schema_dir = g_getenv("GSETTINGS_SCHEMA_DIR");
	if (!schema_dir) {
		schema_dir = "/var/tmp/dbg/share/glib-2.0/schemas";
	}

	if (schema_dir) {
		GError* error = NULL;
		settings_schema_source = g_settings_schema_source_new_from_directory(schema_dir,
			NULL, false, &error);
		if (error){
			wlog_if_level(wll_error, "%s: %s error: %s\n", __func__,
				"g_settings_schema_source_new_from_directory", error->message);
			g_clear_error(&error);
		}
	}
#else
	settings_schema_source = g_settings_schema_source_get_default();
#endif // DEBUG

	return settings_schema_source;
}
