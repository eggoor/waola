noinst_LTLIBRARIES = libwaolasc_s.la
libwaolasc_s_la_SOURCES = \
include/libwaolasc.h \
\
pch.c \
pch.h \
pch_u.h \
waolasc.c \
waolasc.h \
waolasc_factory.c

AM_CFLAGS = \
-I../../libwlog/include \
-I../../libwaolash/include -I../../waolash/include \
-I../../waolasc/include \
-I../../libwaolath/include -I../../libwaolane/include \
-I../../libwaolava/include \
-I../../libwaolast/include -I../../libwaoladi/include \
-I../../libwaolare/include

libwaolasc_s_la_LIBADD = \
../libwlog/libwlog_s.la \
../libwaolash/libwaolash_s.la \
../libwaolath/libwaolath_s.la \
../libwaolane/libwaolane_s.la \
../libwaolava/libwaolava_s.la \
../libwaolast/libwaolast_s.la \
../libwaolare/libwaolare_s.la \
../libwaoladi/libwaoladi_s.la
