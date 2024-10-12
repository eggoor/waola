/** @file host_dlg.h
* WaolaG host dialog declaration.
*/

#pragma once

#include "WaolagHost.h"
#include "waolag.h"

G_BEGIN_DECLS

#define WAOLAG_TYPE_HOST_DLG (waolag_host_dlg_get_type())

G_DECLARE_FINAL_TYPE(WaolagHostDlg, waolag_host_dlg, WAOLAG, HOST_DLG, GtkWindow)

typedef void (*WaolagHostDlgSaveCb)(WaolagMainWnd*, WaolagHostDlg*);
void host_dlg_init(WaolagHostDlg* self, WaolagMainWnd* main_wnd,
	WaolagHostDlgSaveCb cb, WaolagHost* host);

WaolagHost* host_dlg_get_host(WaolagHostDlg* self);

const gchar* host_dlg_get_display_name(WaolagHostDlg* self);
void host_dlg_set_display_name(WaolagHostDlg* self, const gchar* name);

const gchar* host_dlg_get_hostname(WaolagHostDlg* self);
void host_dlg_set_hostname(WaolagHostDlg* self, const gchar* name);

const gchar* host_dlg_get_ip_addr(WaolagHostDlg* self);
void host_dlg_set_ip_addr(WaolagHostDlg* self, const gchar* name);

const gchar* host_dlg_get_mac_addr(WaolagHostDlg* self);
void host_dlg_set_mac_addr(WaolagHostDlg* self, const gchar* name);

void on_host_dlg_button_ok_clicked(GtkButton* button, gpointer user_data);
void on_host_dlg_button_cancel_clicked(GtkButton* button, gpointer user_data);

G_END_DECLS
