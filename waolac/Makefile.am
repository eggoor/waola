bin_PROGRAMS = waolac

waolac_SOURCES = main.c \
pch.c \
pch.h \
pch_u.h \
waolac.c \
waolac.h \
waolac_priv.h

AM_CFLAGS = -I../../libwlog/include -I../../libwaola/include \
 -I../../libwaolash/include -I../../waolash/include \
-I../../libwaolath/include -I../../libwaolane/include \
-I../../libwaolava/include \
-I../../libwaolast/include \
-I../../libwaoladi/include \
-I../../libwaolare/include \
-I../../libwaolasc/include -I../../waolasc/include

waolac_LDFLAGS = -static

waolac_LDADD = \
../libwaola/libwaola_s.la \
../libwaolasc/libwaolasc_s.la \
../libwaoladi/libwaoladi_s.la \
../libwaolare/libwaolare_s.la \
../libwaolava/libwaolava_s.la \
../libwaolane/libwaolane_s.la \
../libwaolath/libwaolath_s.la \
../libwaolast/libwaolast_s.la \
../libwaolash/libwaolash_s.la \
../libwlog/libwlog_s.la

install-exec-hook:
	sudo setcap cap_net_raw+eip $(DESTDIR)$(bindir)/waolac$(EXEEXT)
