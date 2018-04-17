ACLOCAL_AMFLAGS = -I build ${ACLOCAL_FLAGS}

SUBDIRS = .

DISTCHECK_CONFIGURE_FLAGS =
MAINTAINERCLEANFILES =
EXTRA_DIST =
CLEANFILES =
noinst_PROGRAMS =

lib_LTLIBRARIES = out/libinsane-0.1.la

libinsanedir = $(pkgincludedir)/src
libinsane_HEADERS = \
    include/libinsane/capi.h \
    include/libinsane/dumb.h \
    include/libinsane/error.h \
    include/libinsane/log.h \
    include/libinsane/multiplexer.h \
    include/libinsane/normalizers.h \
    include/libinsane/safebet.h \
    include/libinsane/str2impls.h \
    include/libinsane/util.h \
    include/libinsane/workarounds.h

out_libinsane_0_1_la_SOURCES = \
    src/bases/dumb.c \
    src/log.c \
    src/normalizers/all_opts_on_all_sources.c \
    src/normalizers/end_of_feed.c \
    src/normalizers/flatbed_and_feeder_behavior.c \
    src/normalizers/min_one_source.c \
    src/normalizers/raw.c \
    src/normalizers/raw24.c \
    src/normalizers/resolution_constraint_format.c \
    src/normalizers/scan_area_opts.c \
    src/normalizers/source_names.c \
    src/normalizers/source_nodes.c \
    src/normalizers/source_types.c \
    src/normalizers/sources_order.c \
    src/normalizers/strip_non_scanners.c \
    src/safebet.c \
    src/str2impls.c \
    src/workarounds/clean_dev_model_char.c \
    src/workarounds/clean_dev_model_from_manufacturer.c \
    src/workarounds/dedicated_thread.c \
    src/workarounds/no_read_on_inactive_opt.c \
    src/workarounds/no_write_on_readonly_opt.c \
    src/workarounds/opt_doc_source.c \
    src/workarounds/opt_mode.c \
    src/workarounds/opt_scan_resolution.c \
    src/workarounds/opts_page_size.c \
    src/workarounds/strip_translations.c

out_libinsane_0_1_la_CFLAGS = \
    -Iinclude \
    $(DEPS_CFLAGS) \
    $(AM_CFLAGS)

out_libinsane_0_1_la_LIBADD = \
    $(DEPS_LIBS) \
    $(AM_LIBADD)

out_libinsane_0_1_la_LDFLAGS = \
    $(AM_LDFLAGS) -no-undefined

if WINDOWS
    out_libinsane_0_1_la_SOURCES += \
        src/bases/twain.c \
        src/bases/wia_automation.c \
        src/bases/wia_ll.c
else
    out_libinsane_0_1_la_SOURCES += \
        src/bases/sane.c
endif
