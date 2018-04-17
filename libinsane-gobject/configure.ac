AC_PREREQ(2.65)
AC_INIT([libinsane-gobject],[0.1])

AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_SRCDIR([src/libinsane.c])

AC_CONFIG_HEADERS([include/libinsane-gobject/config.h])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_PREREQ([2.2.6])
LT_INIT([])

AM_INIT_AUTOMAKE([1.11 -Wno-portability -Wall foreign no-define subdir-objects])

AC_PROG_CC
AC_HEADER_STDC

PKG_CHECK_MODULES(DEPS, [gobject-introspection-1.0, glib-2.0])
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)
GOBJECT_INTROSPECTION_CHECK([1.29])

build_linux=no
build_windows=no
build_mac=no
AC_MSG_NOTICE(["Host OS: ${host_os}"])
case "${host_os}" in
    linux*)
        build_linux=yes
        ;;
    cygwin*|mingw*)
        build_windows=yes
        ;;
    darwin*)
        build_mac=yes
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac
AM_CONDITIONAL([LINUX], [test "$build_linux" = "yes"])
AM_CONDITIONAL([WINDOWS], [test "$build_windows" = "yes"])
AM_CONDITIONAL([OSX], [test "$build_mac" = "yes"])

CFLAGS="$ENV_CFLAGS $DEPS_CFLAGS -O2 -Wall -I../libinsane/include"
LDFLAGS="$ENV_LDFLAGS $DEPS_LIBS -O2 -Wl,--as-needed -L../libinsane/out/.libs -linsane"

AC_ARG_ENABLE(
	[fatal-warnings],
	[--enable-fatal-warnings : enable fatal warnings (-Werror)],
	[ default="no" ]
)
AC_ARG_ENABLE(
	[debug],
	[--enable-debug : enable debug (-g -ggdb)],
	[ default="no" ]
)

if test "x${enable_fatal_warnings}" = xyes ; then
	AC_MSG_NOTICE([Fatal warnings enabled (-Werror)])
	CFLAGS="$CFLAGS -Werror"
	LDFLAGS="$CFLAGS -Werror"
else
	AC_MSG_NOTICE([Fatal warnings disabled])
fi

if test "x${enable_debug}" = xyes ; then
	AC_MSG_NOTICE([Debug enabled (-g -ggdb)])
	CFLAGS="$CFLAGS -g -ggdb"
	LDFLAGS="$CFLAGS -g -ggdb"
else
	AC_MSG_NOTICE([Debug disabled])
fi

AC_CONFIG_FILES([
  Makefile
])
AC_OUTPUT
