#ifndef __LIBINSANE_ERROR_H
#define __LIBINSANE_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#define LIS_ENUM_ERROR 0xFFFFFFF

#define LIS_ERR_BIT (1 << 30)
#define LIS_ERR_INTERNAL_BIT (1 << 29)

enum lis_error {
	LIS_OK = 0, /*!< Everything is fine. */
	LIS_WARMING_UP, /*!< Lamp must warm up first. */

	LIS_ERR_DEVICE_BUSY = LIS_ERR_BIT, /*!< Device already used by another process. */
	LIS_ERR_CANCELLED, /*!< Scan was cancelled, either by software or by hardware but scan_read()
						* was attempted anyway */
	LIS_ERR_UNSUPPORTED, /*!< Operation is not supported. */
	LIS_ERR_INVALID_VALUE, /*!< Invalid value. */
	LIS_ERR_JAMMED, /*!< Your scanner is a mess. Go unmess it first. */
	LIS_ERR_COVER_OPEN, /*!< Scanner cover is opened. Pleae close the scanner first. */
	LIS_ERR_IO_ERROR, /*!< Basically, things didn't work and we don't know why. */
	LIS_ERR_NO_MEM, /*!< Out of memory */
	LIS_ERR_ACCESS_DENIED, /*!< Operation not allowed. */
	LIS_ERR_HW_IS_LOCKED, /*!< Scanner is locked and doesn't allow scanning */

	/*!
	 * \brief Image format is not supported by LibInsane.
	 */
	LIS_ERR_INTERNAL_IMG_FORMAT_NOT_SUPPORTED = LIS_ERR_BIT | LIS_ERR_INTERNAL_BIT,
	LIS_ERR_INTERNAL_NOT_IMPLEMENTED,
	LIS_ERR_INTERNAL_UNKNOWN_ERROR, /*!< Backend returned an unknown error code */
};

#define LIS_IS_OK(err) (!((err) & LIS_ERR_BIT))
#define LIS_IS_ERROR(err) ((err) & LIS_ERR_BIT)

/*!
 * \brief returns an English message corresponding to the error code.
 * \return a constant (no need to free).
 */
const char *lis_strerror(enum lis_error);

#ifdef __cplusplus
}
#endif

#endif
