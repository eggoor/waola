noinst_PROGRAMS = waolasht

waolasht_SOURCES = main.c pch.c pch.h pch_u.h \
wlist.c wlist.h wcont4r.c wcont4r.h whost.c whost.h \
whost_view.c whost_view.h wmac_addr.c wmac_addr.h \
wsockaddr.c wsockaddr.h

AM_CFLAGS = -I../../libwlog/include -I../../libwaolash/include -I../../waolash/include

waolasht_LDADD = \
../libwlog/libwlog_s.la \
../libwaolash/libwaolash_s.la
