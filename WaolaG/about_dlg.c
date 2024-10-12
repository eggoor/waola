/** @file about_dlg.c
* WaolaG about dialog implementation.
*/

#include "pch.h"

#include "main_wnd.h"

#include "about_dlg.h"
#include "about_dlg_priv.h"

static const char* ma_email = "egggor@gmail.com";

struct _WaolagAboutDlg
{
	GtkWindow parent_instance;

	GtkTextView* appealTextView;
	GtkTextBuffer* appealBuffer;
	GtkTextBuffer* licenseBuffer;
};

G_DEFINE_FINAL_TYPE(WaolagAboutDlg, waolag_about_dlg, GTK_TYPE_WINDOW)

static void waolag_about_dlg_class_init(WaolagAboutDlgClass* klass)
{
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);
	G_OBJECT_CLASS(klass)->dispose = waolag_about_dlg_dispose;

	gtk_widget_class_set_template_from_resource(widget_class,
		"/com/asuscomm/smartnclever/WaolaG/about_dlg.ui");

	gtk_widget_class_bind_template_child(widget_class, WaolagAboutDlg, appealTextView);
	gtk_widget_class_bind_template_child(widget_class, WaolagAboutDlg, appealBuffer);
	gtk_widget_class_bind_template_child(widget_class, WaolagAboutDlg, licenseBuffer);

	GObjectClass* object_class = G_OBJECT_CLASS(klass);
}

static const GActionEntry dlg_actions[] = {
	{ "cancel", dlg_action_cancel },
};

static void waolag_about_dlg_init(WaolagAboutDlg* self)
{
	gtk_widget_init_template(GTK_WIDGET(self));
	init_action_group(self);
	init_event_controllers(self);
	init_views(self);
}

static void init_action_group(WaolagAboutDlg* self)
{
	GActionGroup* actions = (GActionGroup*)g_simple_action_group_new();
	g_action_map_add_action_entries(G_ACTION_MAP(actions), dlg_actions,
		G_N_ELEMENTS(dlg_actions), self);
	gtk_widget_insert_action_group(GTK_WIDGET(self), "dlg", actions);
}

static void init_event_controllers(WaolagAboutDlg* self)
{
	GtkEventController* event_controller = gtk_event_controller_key_new();
	g_signal_connect(event_controller, "key-pressed", G_CALLBACK(on_key_pressed),
		self->appealTextView);
	gtk_widget_add_controller(GTK_WIDGET(self->appealTextView), event_controller);

    event_controller = GTK_EVENT_CONTROLLER(gtk_gesture_click_new());
	g_signal_connect(event_controller, "released", G_CALLBACK(on_mouse_button_released),
		self->appealTextView);
	gtk_widget_add_controller(GTK_WIDGET(self->appealTextView), event_controller);

	event_controller = gtk_event_controller_motion_new();
	g_signal_connect (event_controller, "motion", G_CALLBACK(on_mouse_motion),
		self->appealTextView);
	gtk_widget_add_controller(GTK_WIDGET(self->appealTextView), event_controller);
}

static gboolean on_key_pressed(GtkEventController* event_controller, guint keyval,
	guint keycode, GdkModifierType modifiers, GtkWidget* text_view)
{
	GtkTextIter iter;
	GtkTextBuffer* buffer;

	switch (keyval)
	{
	case GDK_KEY_Return:
	case GDK_KEY_KP_Enter:
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
		gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));
		follow_if_link(text_view, &iter);
		break;
	default:
		break;
	}

	return GDK_EVENT_PROPAGATE;
}

static void on_mouse_button_released(GtkGestureClick *gesture, guint n_press,
	double x, double y, GtkWidget* text_view)
{
	if (gtk_gesture_single_get_button(GTK_GESTURE_SINGLE(gesture)) > 1)	{
		return;
	}

	int tx, ty;
	gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(text_view),
		GTK_TEXT_WINDOW_WIDGET, x, y, &tx, &ty);

	GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

	GtkTextIter start, end, iter;
	gtk_text_buffer_get_selection_bounds(buffer, &start, &end);
	if (gtk_text_iter_get_offset(&start) != gtk_text_iter_get_offset(&end)) {
		return;
	}

	if (gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(text_view), &iter, tx, ty)) {
		follow_if_link(text_view, &iter);
	}
}

static void on_mouse_motion(GtkEventControllerMotion* event_controller,
	double x, double y, GtkTextView* text_view)
{
	int tx, ty;
	gtk_text_view_window_to_buffer_coords(text_view, GTK_TEXT_WINDOW_WIDGET, x, y, &tx, &ty);
	set_cursor_if_appropriate(text_view, tx, ty);
}

static gboolean hovering_over_link = FALSE;

static void set_cursor_if_appropriate(GtkTextView* text_view, int x, int y)
{
	gboolean hovering = FALSE;
	GtkTextIter iter;

	if (gtk_text_view_get_iter_at_location(text_view, &iter, x, y)) {
		GSList* tags = gtk_text_iter_get_tags(&iter);
		for (GSList* tagp = tags; tagp != NULL; tagp = tagp->next) {
			if (tagp->data) {
				hovering = TRUE;
				break;
			}
		}

		if (tags) {
			g_slist_free(tags);
		}
	}

	if (hovering != hovering_over_link) {
		hovering_over_link = hovering;

		if (hovering_over_link) {
			gtk_widget_set_cursor_from_name(GTK_WIDGET(text_view),
				"pointer");
		}
		else {
			gtk_widget_set_cursor_from_name(GTK_WIDGET(text_view),
				"text");
		}
	}
}

static void follow_if_link(GtkWidget* text_view, GtkTextIter* iter)
{
	GSList* tags = gtk_text_iter_get_tags(iter);
	for (GSList* tagp = tags; tagp != NULL; tagp = tagp->next) {
		GtkTextTag* tag = tagp->data;
		if (tag) {
			int rc = system("xdg-open https://paypal.com/ua/home");
			if (0 != rc) {
				wlog_if_level(wll_warning, "Unable to 'xdg-open https://paypal.com/ua/home', rc == %d\n", rc);
			}
			break;
		}
	}

	if (tags) {
		g_slist_free(tags);
	}
}

static void init_views(WaolagAboutDlg* self)
{
	init_view_appeal(self);
	init_view_license(self);
}

static void init_view_appeal(WaolagAboutDlg* self)
{
	GBytes* bytes = g_resources_lookup_data("/com/asuscomm/smartnclever/WaolaG/../APPEAL.txt",
		G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);

	gsize len;
	const gchar* the_appeal = g_bytes_get_data(bytes, &len);
	gtk_text_buffer_set_text(self->appealBuffer, the_appeal, len);

	GtkTextTag* tag = gtk_text_buffer_create_tag(self->appealBuffer, NULL,
		"foreground", "blue", "underline", PANGO_UNDERLINE_SINGLE, NULL);

	GtkTextIter start, end;
	gtk_text_buffer_get_iter_at_offset(self->appealBuffer, &start, 0);

	if (gtk_text_iter_forward_search(&start, ma_email, 0, &start, &end, NULL)) {
		gtk_text_buffer_apply_tag(self->appealBuffer, tag, &start, &end);
	}

	g_bytes_unref(bytes);
}

static void init_view_license(WaolagAboutDlg* self)
{
	GBytes* bytes = g_resources_lookup_data("/com/asuscomm/smartnclever/WaolaG/../LICENSE.txt",
		G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
	gsize len;
	const gchar* license_text = g_bytes_get_data(bytes, &len);
	gtk_text_buffer_set_text(self->licenseBuffer, license_text, len);
	g_bytes_unref(bytes);
}

static void waolag_about_dlg_dispose(GObject* gobject)
{
  gtk_widget_dispose_template(GTK_WIDGET(gobject), WAOLAG_TYPE_ABOUT_DLG);
  G_OBJECT_CLASS(waolag_about_dlg_parent_class)->dispose(gobject);
}

void about_dlg_init()
{
	const gchar* val = NULL;
	GValue gval = G_VALUE_INIT;

	g_value_init (&gval, G_TYPE_STRING);
}

static void dlg_action_cancel(GSimpleAction* action,
	GVariant* parameter, gpointer user_data)
{
	GtkWindow* self = user_data;
	g_assert(WAOLAG_IS_ABOUT_DLG(self));
	gtk_window_close(self);
}
