#include <assert.h>

#include <libinsane-gobject/error.h>
#include <libinsane-gobject/error_private.h>


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


LibinsaneError lis_error_to_gobject(enum lis_error err)
{
	switch(err) {
		case LIS_OK:
			return LIBINSANE_ERROR_OK;
		case LIS_ERR_CANCELLED:
			return LIBINSANE_ERROR_CANCELLED;
		case LIS_WARMING_UP:
			/* shouldn't be arrive here */
			assert(err != LIS_WARMING_UP);
			break;
		case LIS_ERR_DEVICE_BUSY:
			return LIBINSANE_ERROR_DEVICE_BUSY;
		case LIS_ERR_UNSUPPORTED:
			return LIBINSANE_ERROR_UNSUPPORTED;
		case LIS_ERR_INVALID_VALUE:
			return LIBINSANE_ERROR_INVALID_VALUE;
		case LIS_ERR_JAMMED:
			return LIBINSANE_ERROR_JAMMED;
		case LIS_ERR_COVER_OPEN:
			return LIBINSANE_ERROR_COVER_OPEN;
		case LIS_ERR_IO_ERROR:
			return LIBINSANE_ERROR_IO_ERROR;
		case LIS_ERR_NO_MEM:
			return LIBINSANE_ERROR_NO_MEM;
		case LIS_ERR_ACCESS_DENIED:
			return LIBINSANE_ERROR_ACCESS_DENIED;
		case LIS_ERR_HW_IS_LOCKED:
			return LIBINSANE_ERROR_HW_IS_LOCKED;
		case LIS_ERR_INTERNAL_IMG_FORMAT_NOT_SUPPORTED:
			return LIBINSANE_ERROR_INTERNAL_IMG_FORMAT_NOT_SUPPORTED;
		case LIS_ERR_INTERNAL_NOT_IMPLEMENTED:
			return LIBINSANE_ERROR_INTERNAL_NOT_IMPLEMENTED;
		case LIS_ERR_INTERNAL_UNKNOWN_ERROR:
			return LIBINSANE_ERROR_INTERNAL_UNKNOWN_ERROR;
	}
	return LIBINSANE_ERROR_INTERNAL_NOT_IMPLEMENTED;
}
