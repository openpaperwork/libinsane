#ifndef __LIBINSANE_ERROR_H
#define __LIBINSANE_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

enum lis_error {
	LIS_OK = 0;
	LIS_CANCELLED,
	LIS_END_OF_PAGE,
	LIS_END_OF_FEED,
	LIS_WARMING_UP,
	LIS_APPROXIMATE_VALUE, /*!< value set on the option is not exactly the one provided */

	LIS_ERR_DEVICE_BUSY = (1 << 31),
	LIS_ERR_UNSUPPORTED,
	LIS_ERR_INVALID_VALUE,
	LIS_ERR_JAMMED,
	LIS_ERR_COVER_OPEN,
	LIS_ERR_IO_ERROR,
	LIS_ERR_NO_MEM,
	LIS_ERR_ACCESS_DENIED,
	LIS_ERR_HW_IS_LOCKED,

	LIS_ERR_INTERNAL_FORMAT_NOT_SUPPORTED = (1 << 31) | (1 << 30),
};

#define LIS_IS_OK(err) ((err) == LIS_OK)
#define LIS_IS_ERROR(err) ((err) & (1 << 31))

const char *lis_strerror(enum lis_error);

#ifdef __cplusplus
}
#endif

#endif
