noinst_PROGRAMS = waoladit

waoladit_SOURCES = main.c pch.c pch.h pch_u.h pch_l.h \
stopwatch.h stopwatch.c stopwatch_u.c threads.c threads.h \
mem.c mem.h  

AM_CFLAGS = -I../../libwlog/include -I../../libwaola/include \
 -I../../libwaolash/include -I../../waolash/include \
-I../../libwaolath/include -I../../libwaolane/include \
-I../../libwaolava/include \
-I../../libwaolast/include \
-I../../libwaoladi/include \
-I../../libwaolare/include \
-I../../libwaolasc/include -I../../waolasc/include

waoladit_LDADD = ../libwaolash/libwaolash_s.la \
../libwlog/libwlog_s.la \
../libwaolath/libwaolath_s.la \
../libwaolane/libwaolane_s.la \
../libwaolasc/libwaolasc_s.la
