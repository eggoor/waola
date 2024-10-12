lib_LTLIBRARIES = libwaolasc.la
libwaolasc_la_SOURCES = \
pch.c \
pch_u.h \
pch.h \
waolasc.c

pkgconfigdir=$(libdir)/pkgconfig

pkgconfig_DATA=waolasc@PACKAGE_SUFFIX@.pc

waolasc@PACKAGE_SUFFIX@.pc:	waolasc.pc
	cp $< $@

waolascincludedir = $(includedir)/waolasc

waolascinclude_HEADERS = \
include/waolasc_exports_u.h \
include/waolasc_exports.h \
include/waolasc_t.h \
include/waolasc.h

AM_CFLAGS = -I../../libwlog/include \
-I../../libwaolash/include -I../../waolash/include \
-I../../libwaolath/include -I../../libwaolast/include \
-I../../libwaolava/include -I../../libwaolavad/include \
-I../../libwaoladi/include -I../../libwaolane/include \
-I../../libwaolare/include \
-I../../libwaolasc/include -I../../waolasc/include

libwaolasc_la_LIBADD = \
../libwaolasc/libwaolasc_s.la

libwaolasc_la_LDFLAGS = --whole-archive,-version-info 0:0:0

EXTRA_DIST= \
	waolasc.pc.in \
	$(NULL)

CLEANFILES= \
	waolasc@PACKAGE_SUFFIX@.pc \
	$(NULL)
