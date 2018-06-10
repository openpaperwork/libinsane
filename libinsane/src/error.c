#include <stdlib.h>

#include <libinsane/error.h>


const char *lis_strerror(enum lis_error err)
{
	switch(err) {
		case LIS_OK:
			return "Success";
		case LIS_WARMING_UP:
			return "Device is warming up";
		case LIS_ERR_CANCELLED:
			return "Operation cancelled";
		case LIS_ERR_DEVICE_BUSY:
			return "Device busy";
		case LIS_ERR_UNSUPPORTED:
			return "Operation not supported";
		case LIS_ERR_INVALID_VALUE:
			return "Invalid value";
		case LIS_ERR_JAMMED:
			return "Device jammed";
		case LIS_ERR_COVER_OPEN:
			return "Device cover is opened";
		case LIS_ERR_IO_ERROR:
			return "I/O Error";
		case LIS_ERR_NO_MEM:
			return "Out of memory";
		case LIS_ERR_ACCESS_DENIED:
			return "Access denied";
		case LIS_ERR_HW_IS_LOCKED:
			return "Hardware is locked (used by another application ?)";
		case LIS_ERR_INTERNAL_IMG_FORMAT_NOT_SUPPORTED:
			return "LibInsane internal error: Image format not supported (please report !)";
		case LIS_ERR_INTERNAL_NOT_IMPLEMENTED:
			return "LibInsane internal error: Operation not implemented (please report !)";
		case LIS_ERR_INTERNAL_UNKNOWN_ERROR:
			return "LibInsane internal error: Unknown error reported by backend (please report !)";
	}
	return NULL;
}
