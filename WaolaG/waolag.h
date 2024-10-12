#pragma once

G_BEGIN_DECLS

#define WAOLAG_TYPE_APPLICATION (waolag_get_type())
G_DECLARE_FINAL_TYPE(WaolaG, waolag, WAOLAG, APPLICATION, GtkApplication)

WaolaG *waolag_new(const char* application_id, GApplicationFlags flags);

G_END_DECLS
