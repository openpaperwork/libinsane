#ifndef __LIBINSANE_GOBJECT_ERROR_PRIVATE_H
#define __LIBINSANE_GOBJECT_ERROR_PRIVATE_H

#include <libinsane/error.h>

#include "error.h"

#define SET_LIBINSANE_GOBJECT_ERROR(gerror, libinsane_err, msg, ...) \
	g_set_error( \
		(gerror), \
		LIBINSANE_ERROR, lis_error_to_gobject(libinsane_err), \
		msg, __VA_ARGS__ \
	);

LibinsaneError lis_error_to_gobject(enum lis_error err);

#endif
