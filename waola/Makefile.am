lib_LTLIBRARIES = libwaola.la

libwaola_la_SOURCES = \
pch.c \
pch.h \
pch_u.h \
waola.c

pkgconfigdir=$(libdir)/pkgconfig

pkgconfig_DATA=waola@PACKAGE_SUFFIX@.pc

waola@PACKAGE_SUFFIX@.pc:	waola.pc
	cp $< $@

pkginclude_HEADERS = \
include/waola_exports_u.h \
include/waola_exports.h \
include/waola.h

AM_CFLAGS = -I../../libwlog/include \
-I../../libwaola/include \
-I../../libwaolash/include -I../../waolash/include \
-I../../libwaolath/include -I../../libwaolane/include

libwaola_la_LIBADD = \
../libwaola/libwaola_s.la

#../waolashd/libwaolash.la

libwaola_la_LDFLAGS = --whole-archive,-version-info 0:0:0

EXTRA_DIST=			\
	waola.pc.in	\
	$(NULL)

CLEANFILES=						\
	waola@PACKAGE_SUFFIX@.pc	\
	$(NULL)
