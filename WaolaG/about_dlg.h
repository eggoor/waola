/** @file about_dlg.h
* WaolaG about dialog declaration.
*/

#pragma once

#include "waolag.h"

G_BEGIN_DECLS

#define WAOLAG_TYPE_ABOUT_DLG (waolag_about_dlg_get_type())

G_DECLARE_FINAL_TYPE(WaolagAboutDlg, waolag_about_dlg, WAOLAG, ABOUT_DLG, GtkWindow)

void on_about_dlg_button_cancel_clicked(GtkButton* button, gpointer user_data);

G_END_DECLS
