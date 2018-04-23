ACLOCAL_AMFLAGS = -I build ${ACLOCAL_FLAGS}

SUBDIRS = .

MAINTAINERCLEANFILES =
noinst_PROGRAMS =

lib_LTLIBRARIES = out/libinsane-gobject-0.1.la

libinsane_gobjectdir = $(pkgincludedir)
libinsane_gobject_HEADERS = \
    include/libinsane-gobject/libinsane-api.h

out_libinsane_gobject_0_1_la_SOURCES = \
    src/libinsane-api.c

out_libinsane_gobject_0_1_la_CFLAGS = \
    -Iinclude \
    $(DEPS_CFLAGS) \
    $(AM_CFLAGS)

out_libinsane_gobject_0_1_la_LIBADD = \
    $(DEPS_LIBS) \
    $(AM_LIBADD)

out_libinsane_gobject_0_1_la_LDFLAGS = \
    $(AM_LDFLAGS) -no-undefined

# Introspection
-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_SCANNER_ARGS = --warn-all --warn-error
INTROSPECTION_COMPILER_ARGS =
DISTCHECK_CONFIGURE_FLAGS = --enable-introspection
EXTRA_DIST = introspection.m4

out/Libinsane-1.0.gir: out/libinsane-gobject-0.1.la
out_Libinsane_1_0_gir_INCLUDES = GLib-2.0
out_Libinsane_1_0_gir_CFLAGS = $(DEPS_CFLAGS) $(src_libinsane_gobject_0_1_la_CFLAGS)
out_Libinsane_1_0_gir_LIBS = out/libinsane-gobject-0.1.la
out_Libinsane_1_0_gir_FILES = $(out_libinsane_gobject_0_1_la_SOURCES) $(libinsane_gobject_HEADERS)
out_Libinsane_1_0_gir_NAMESPACE = Libinsane
out_Libinsane_1_0_gir_EXPORT_PACKAGES = libinsane
out_Libinsane_1_0_gir_SCANNERFLAGS = 	\
	--nsversion=1.0 		\
	--symbol-prefix=insane \
	--symbol-prefix=libinsane

INTROSPECTION_GIRS = out/Libinsane-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelib_DATA)
