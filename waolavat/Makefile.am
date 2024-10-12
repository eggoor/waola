noinst_PROGRAMS = waolavat
waolavat_SOURCES = main.c pch.c pch.h pch_u.h \
waolavat.c waolavat.h

AM_CFLAGS = \
-I../../libwlog/include \
-I../../libwaolash/include \
-I../../waolash/include \
-I../../libwaolath/include \
-I../../libwaolava/include

waolavat_LDADD = \
../libwaolava/libwaolava_s.la \
../libwaolash/libwaolash_s.la \
../libwaolath/libwaolath_s.la \
../libwlog/libwlog_s.la
