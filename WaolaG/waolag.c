#include "pch.h"

#include "main_wnd.h"
#include "about_dlg.h"
#include "waolag.h"

struct _WaolaG
{
	GtkApplication parent_instance;
};

G_DEFINE_TYPE (WaolaG, waolag, GTK_TYPE_APPLICATION)

WaolaG* waolag_new(const char* application_id,
                    GApplicationFlags flags)
{
	g_return_val_if_fail(application_id != NULL, NULL);

	return g_object_new (WAOLAG_TYPE_APPLICATION, "application-id",
		application_id, "flags", flags, NULL);
}

static void waolag_activate(GApplication* app)
{
	GtkWindow* window;
	g_assert(WAOLAG_IS_APPLICATION(app));

	window = gtk_application_get_active_window(GTK_APPLICATION(app));

	if (!window) {
		window = g_object_new(WAOLAG_TYPE_MAIN_WND, "application", app, NULL);
	}

	gtk_window_present(window);
}

static void waolag_class_init(WaolaGClass* klass)
{
	GApplicationClass* app_class = G_APPLICATION_CLASS(klass);

	app_class->activate = waolag_activate;
}

static void waolag_about_action(GSimpleAction* action, GVariant* parameter,
	gpointer user_data)
{
	static const char* developers[] = {"egggor@gmail.com", NULL};
	WaolaG* self = user_data;
	g_assert(WAOLAG_IS_APPLICATION(self));

	GtkWindow* window = gtk_application_get_active_window(GTK_APPLICATION(self));

	GtkWindow* about_dlg = g_object_new(WAOLAG_TYPE_ABOUT_DLG, "application", self, NULL);
	gtk_window_present(about_dlg);
	gtk_window_set_transient_for(about_dlg, window);
	gtk_window_set_modal(about_dlg, true);
}

static void waolag_quit_action (GSimpleAction* action, GVariant *parameter,
	gpointer user_data)
{
	WaolaG* self = user_data;
	g_assert(WAOLAG_IS_APPLICATION(self));

	g_application_quit(G_APPLICATION(self));
}

static const GActionEntry app_actions[] = {
	{ "quit", waolag_quit_action },
	{ "about", waolag_about_action },
};

static void waolag_init(WaolaG* self)
{
	g_action_map_add_action_entries(G_ACTION_MAP (self),
		app_actions, G_N_ELEMENTS (app_actions), self);
	gtk_application_set_accels_for_action (GTK_APPLICATION (self), "app.quit",
	 (const char*[]) { "<primary>q", NULL });
}
