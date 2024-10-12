noinst_PROGRAMS = waolanet

waolanet_SOURCES = main.c pch.c pch.h pch_u.h \
wnet_iface.c wnet_iface.h wnet_iface_prov.c wnet_iface_prov.h

AM_CFLAGS = -I../../libwlog/include \
-I../../libwaolash/include -I../../waolash/include \
-I../../libwaolath/include -I../../libwaolane/include

waolanet_LDADD = \
../libwaolane/libwaolane_s.la \
../libwaolath/libwaolath_s.la \
../libwaolash/libwaolash_s.la \
../libwlog/libwlog_s.la
