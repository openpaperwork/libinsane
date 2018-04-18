ACLOCAL_AMFLAGS = -I build ${ACLOCAL_FLAGS}

SUBDIRS = .

MAINTAINERCLEANFILES =
noinst_PROGRAMS =

lib_LTLIBRARIES = out/libinsane-gobject-0.1.la

libinsane_gobjectdir = $(pkgincludedir)/src
libinsane_gobject_HEADERS = \
    include/libinsane-gobject/libinsane-api.h

out_libinsane_gobject_0_1_la_SOURCES = \
    src/libinsane.c

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
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --warn-all
INTROSPECTION_COMPILER_ARGS =
DISTCHECK_CONFIGURE_FLAGS = --enable-introspection
EXTRA_DIST = introspection.m4

src/Insane-1.0.gir: out/libinsane-gobject-0.1.la
src_Insane_1_0_gir_INCLUDES = GLib-2.0
src_Insane_1_0_gir_CFLAGS = $(DEPS_CFLAGS) $(src_libinsane_gobject_0_1_la_CFLAGS)
src_Insane_1_0_gir_LIBS = out/libinsane-gobject-0.1.la
src_Insane_1_0_gir_FILES = $(src_libinsane_gobject_0_1_la_SOURCES) $(libinsane_gobject_HEADERS)
src_Insane_1_0_gir_NAMESPACE = Insane
src_Insane_1_0_gir_EXPORT_PACKAGES = libinsane
src_Insane_1_0_gir_SCANNERFLAGS = 	\
	--nsversion=1.0 		\
	--symbol-prefix=insane 	\
	--c-include="libinsane-gobject/include/libinsane-gobject.h"

INTROSPECTION_GIRS += src/Insane-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelib_DATA)
