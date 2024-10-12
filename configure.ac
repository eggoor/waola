#                -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_INIT([waola],[0.1],[egggor@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2 no-dist-gzip foreign])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([libwlog/pch.c])
AC_CONFIG_SRCDIR([libwaolash/pch.c])
AC_CONFIG_SRCDIR([libwaolath/pch.c])
AC_CONFIG_SRCDIR([libwaolava/pch.c])
AC_CONFIG_SRCDIR([libwaolast/pch.c])
AC_CONFIG_SRCDIR([libwaolare/pch.c])
AC_CONFIG_SRCDIR([libwaoladi/pch.c])
AC_CONFIG_SRCDIR([libwaolane/pch.c])
AC_CONFIG_SRCDIR([libwaolasc/pch.c])
AC_CONFIG_SRCDIR([libwaola/pch.c])
AC_CONFIG_SRCDIR([waolash/pch.c])
AC_CONFIG_SRCDIR([waola/pch.c])
AC_CONFIG_SRCDIR([waolasc/pch.c])
AC_CONFIG_SRCDIR([waolasht/pch.c])
AC_CONFIG_SRCDIR([waoladit/pch.c])
AC_CONFIG_SRCDIR([waolavat/pch.c])
AC_CONFIG_SRCDIR([waolanet/pch.c])
AC_CONFIG_SRCDIR([waolac/pch.c])
AC_CONFIG_SRCDIR([WaolaG/pch.c])

AC_CONFIG_HEADERS([config.h])

ABI_VERSION=0.1
AC_SUBST(ABI_VERSION)
AC_DEFINE_UNQUOTED(ABI_VERSION, "$ABI_VERSION", [ABI version of the package])
PACKAGE_SUFFIX="-${ABI_VERSION}"
AC_SUBST(PACKAGE_SUFFIX)
AC_DEFINE_UNQUOTED(PACKAGE_SUFFIX, "$PACKAGE_SUFFIX", [Suffix of the package])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR
LT_INIT
AC_PROG_CC
PKG_PROG_PKG_CONFIG

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([malloc.h stdlib.h stddef.h string.h stdint.h stdarg.h stdio.h \
time.h limits.h math.h errno.h sysexits.h inttypes.h netdb.h unistd.h fcntl.h pthread.h \
sys/socket.h sys/ioctl.h net/if.h net/ethernet.h netinet/in.h netinet/if_ether.h \
arpa/inet.h linux/if_packet.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset socket])

AC_LANG([C])

AC_ARG_ENABLE([gtk],
  [AS_HELP_STRING([--disable-gtk], [Enable building of gtk4 GUI frontend @<:@check@:>@])],
  [:],
  [enable_gtk=check])

WAOLASH_VERSION=0.1
AC_SUBST(WAOLASH_VERSION)
WAOLASC_VERSION=0.1
AC_SUBST(WAOLASC_VERSION)
WAOLA_VERSION=0.1
AC_SUBST(WAOLA_VERSION)

AS_IF([test "$enable_gtk" != "no"],
  [PKG_CHECK_MODULES([GTK],
	[gtk4],
	[enable_gtk=yes],
	[AS_IF([test "$enable_gtk" = "yes"],
	  [AC_MSG_ERROR([Building of gtk4 GUI frontend is requested but library not found.])],
	  [enable_gtk=no])])])

AM_CONDITIONAL([ENABLE_GTK], [test "$enable_gtk" = "yes"])
AM_COND_IF([ENABLE_GTK], [GLIB_GSETTINGS])

AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
	[debug=false])

AS_IF([test x"$debug" = x"true"], [AC_DEFINE([DEBUG], [1], [debug build])])

AM_CONDITIONAL([DEBUG], [test x"$debug" = x"true"])

AC_CONFIG_FILES([
Makefile
libwaolast/Makefile
libwaolath/Makefile
libwaolava/Makefile
libwlog/Makefile
libwaolane/Makefile
libwaolare/Makefile
libwaolash/Makefile
libwaolasc/Makefile
libwaoladi/Makefile
libwaola/Makefile
waolac/Makefile
waolasc/waolasc.pc
waolasc/Makefile
waola/waola.pc
waola/Makefile
waoladit/Makefile
waolavat/Makefile
waolasht/Makefile
waolanet/Makefile
waolash/waolash.pc
waolash/Makefile
])

AM_COND_IF([ENABLE_GTK], [AC_CONFIG_FILES([WaolaG/Makefile])])

AC_OUTPUT
