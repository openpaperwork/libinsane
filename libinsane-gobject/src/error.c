#include <libinsane-gobject/error.h>

/**
 * LIBINSANE_ERROR:
 *
 * Error domain for libinsane operations. Errors in this domain will
 * be from the #LibinsaneError enumeration. See #GError for information
 * on error domains.
 */

GQuark libinsane_error_quark(void)
{
	static GQuark q = 0;
	if (q == 0) {
		q = g_quark_from_static_string("libinsane-quark");
	}
	return q;
}
