bin_PROGRAMS = waolag

waolag_SOURCES = \
about_dlg_priv.h \
about_dlg.c \
about_dlg.h \
about_dlg.ui \
host_dlg_priv.h \
host_dlg.c \
host_dlg.h \
host_dlg.ui \
main_wnd.c \
main_wnd.h \
main_wnd_priv.h \
main_wnd.ui \
WaolagHost.c \
WaolagHost.h \
main.c \
pch.c \
pch.h \
waola_event_data.h \
waolag.h \
waolag.c \
waolag.gresource.xml \
resources.c \
WaolaG.desktop.in \
../Graphics/01wakeup.svg \
../APPEAL.txt \
../LICENSE.txt

AM_CFLAGS = `pkg-config --cflags gtk4` \
-I../../libwlog/include -I../../libwaolath/include \
-I../../libwaolava/include -I../../libwaolast/include \
-I../../waolash/include -I../../waolasc/include \
-I../../waola/include -I../../libwaolane/include \
-DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED

waolag_LDFLAGS = -static `pkg-config --libs gtk4`

waolag_LDADD = \
../libwlog/libwlog_s.la \
../waolash/libwaolash.la \
../waolasc/libwaolasc.la \
../waola/libwaola.la

desktopdir = $(datadir)/applications
desktop_in_files = WaolaG.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)

$(desktop_DATA): $(desktop_in_files)
	$(AM_V_GEN) cp $< $@

iconsdir = $(datadir)/icons/hicolor/scalable/apps
icon_in_files = ../../Graphics/01wakeup.svg
icons_DATA = Waola.svg

$(icons_DATA): $(icon_in_files)
	$(AM_V_GEN) cp $< $@

CLEANFILES = resources.c $(desktop_DATA) $(icons_DATA)

resources.c:
	glib-compile-resources ../../WaolaG/waolag.gresource.xml --sourcedir=../../WaolaG --target=resources.c --generate-source

gsettings_SCHEMAS = com.asuscomm.smartnclever.WaolaG.gschema.xml
EXTRA_DIST = $(gsettings_SCHEMAS)

@GSETTINGS_RULES@

install-exec-hook:
	sudo setcap cap_net_raw+eip $(DESTDIR)$(bindir)/waolag$(EXEEXT)
