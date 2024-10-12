/** @file main_wnd.h
* WaolaG main window declaration.
*/
#pragma once

#include "waolag.h"

G_BEGIN_DECLS

#define WAOLAG_TYPE_MAIN_WND (waolag_main_wnd_get_type())

G_DECLARE_FINAL_TYPE(WaolagMainWnd, waolag_main_wnd, WAOLAG, MAIN_WND, GtkApplicationWindow)

G_END_DECLS
