AC_PREREQ(2.65)
AC_INIT([libinsane],[0.1])

AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_SRCDIR([src/safebet.c])

AC_CONFIG_HEADERS([include/libinsane/config.h])

AM_INIT_AUTOMAKE([1.11 -Wall foreign no-define subdir-objects])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_PREREQ([2.2.6])
LT_INIT([])

AC_PROG_CC
AC_HEADER_STDC

# PKG_CHECK_MODULES(DEPS, [gobject-introspection-1.0, glib-2.0])
# AC_SUBST(DEPS_CFLAGS)
# AC_SUBST(DEPS_LIBS)
# GOBJECT_INTROSPECTION_CHECK([1.29])

AC_CONFIG_FILES([
  Makefile
  libinsane.pc
])
AC_OUTPUT
