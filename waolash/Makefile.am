lib_LTLIBRARIES = libwaolash.la

libwaolash_la_SOURCES = \
pch.c \
pch.h \
pch_l.h \
pch_u.h \
waolash_fact4s.c \
wcont4r.c \
whost.c \
whost_data.c \
whost_view.c \
wlist.c \
wmem.c \
wsockaddr.c \
wstr.c

pkgconfigdir=$(libdir)/pkgconfig

pkgconfig_DATA=waolash@PACKAGE_SUFFIX@.pc

waolash@PACKAGE_SUFFIX@.pc:	waolash.pc
	cp $< $@

waolashincludedir = $(includedir)/waolash

waolashinclude_HEADERS = \
include/waoladi_fact4s_sh.h \
include/waoladi_fact4s_t.h \
include/waoladi_factory.h \
include/waoladi_opcode.h \
include/waoladi_sh.h \
include/waoladi_t.h \
include/waolane_fact4s_sh.h \
include/waolane_fact4s_t.h \
include/waolane_sh.h \
include/waolare_fact4s_sh.h \
include/waolare_factory.h \
include/waolare_opcode.h \
include/waolare_sh.h \
include/waolare_t.h \
include/waolasc_callback.h \
include/waolasc_factory.h \
include/waolasc_opcode.h \
include/waolasc_sh.h \
include/waolasc_t.h \
include/waolash.h \
include/waolash_exports.h \
include/waolash_exports_u.h \
include/waolash_fact4s_sh.h \
include/waolash_t.h \
include/waolast_factory.h \
include/waolast_sh.h \
include/waolast_t.h \
include/waolath_fact4s_sh.h \
include/waolath_sh.h \
include/waolava_callback.h \
include/waolava_fact4s_sh.h \
include/waolava_factory.h \
include/waolava_merge_host_cbi_factory.h \
include/waolava_merge_host_cbi_t.h \
include/waolava_on_host_modified_cbi_factory.h \
include/waolava_on_host_modified_cbi_t.h \
include/waolava_opcode.h \
include/waolava_sh.h \
include/waolava_t.h \
include/warper_cb_data_t.h \
include/wasync_callback.h \
include/wcont4r.h \
include/wcont4r_factory.h \
include/wcont4r_type.h \
include/wdefs.h \
include/wdiscoverer_host_factory.h \
include/wdiscoverer_iface_factory.h \
include/wdiscoverer_iface_monitor_factory.h \
include/wdiscoverer_net_factory.h \
include/wevent_factory.h \
include/wevent_t.h \
include/whost.h \
include/whost_changed_field.h \
include/whost_data.h \
include/whost_factory.h \
include/whost_field.h \
include/whost_op_result.h \
include/whost_view.h \
include/whost_view_factory.h \
include/wlist.h \
include/wmem.h \
include/wmodule.h \
include/wmutex_factory.h \
include/wmutex_t.h \
include/wnet_iface_factory.h \
include/wnet_iface_t.h \
include/wnet_iface_prov_factory.h \
include/wnet_iface_prov_t.h \
include/wresolver_host_factory.h \
include/wresolver_host_t.h \
include/wresolver_hosts_factory.h \
include/wresolver_hosts_t.h \
include/wsockaddr.h \
include/wstr.h \
include/wtask.h \
include/wthread_cbi_factory.h \
include/wthread_cbi_t.h \
include/wthread_counter_factory.h \
include/wthread_counter_t.h \
include/wthread_factory.h \
include/wthread_t.h

AM_CFLAGS = -I../../libwlog/include -I../../libwaolash/include  -I../../waolash/include

libwaolash_la_LIBADD = \
../libwlog/libwlog_s.la \
../libwaolash/libwaolash_s.la

libwaolash_la_LDFLAGS = --whole-archive,-version-info 0:0:0

EXTRA_DIST= \
	waolash.pc.in \
	$(NULL)

CLEANFILES= \
	waolash@PACKAGE_SUFFIX@.pc \
	$(NULL)
