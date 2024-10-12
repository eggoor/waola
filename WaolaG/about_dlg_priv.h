/** @file about_dlg_priv.h
* WaolaG about dialog private declarations.
*/
#pragma once

static void init_action_group(WaolagAboutDlg* self);
static void init_views(WaolagAboutDlg* self);
static void init_view_appeal(WaolagAboutDlg* self);
static void init_view_license(WaolagAboutDlg* self);
static void init_event_controllers(WaolagAboutDlg* self);

static gboolean on_key_pressed(GtkEventController* controller, guint keyval,
	guint keycode, GdkModifierType modifiers, GtkWidget* text_view);
static void on_mouse_button_released(GtkGestureClick *gesture, guint n_press,
	double x, double y, GtkWidget* text_view);
static void on_mouse_motion(GtkEventControllerMotion* controller,
	double x, double y, GtkTextView* text_view);
static void set_cursor_if_appropriate(GtkTextView* text_view, int x, int y);
static void follow_if_link(GtkWidget* text_view, GtkTextIter* iter);

static void waolag_about_dlg_dispose(GObject* gobject);

static void dlg_action_cancel(GSimpleAction* action, GVariant* parameter,
	gpointer user_data);
