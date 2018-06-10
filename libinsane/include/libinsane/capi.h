#ifndef __LIBINSANE_CAPI_H
#define __LIBINSANE_CAPI_H

#include <stdlib.h>

#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Basic informations regarding a scanner.
 */
struct lis_device_descriptor {
	struct lis_api *impl;

	/*!
	 * \brief Device id to use to identify and access the scanner.
	 *
	 * There is strictly no guarantee regarding the content of this string.
	 */
	char *dev_id;

	char *vendor; /*!< Device manufacturer name (ex: "Hewlett-Packard") */
	char *model; /*!< Device model name */
	char *type; /*!< ex: "flatbed scanner", "sheetfed scanner" */
};

/*!
 * \brief Variable type.
 */
enum lis_value_type {
	LIS_TYPE_BOOL = 0, /*!< int: 0 (false) or 1 (true)*/
	LIS_TYPE_INTEGER, /*!< int */
	LIS_TYPE_DOUBLE, /*!< double */
	LIS_TYPE_STRING, /*!< (char *) */
	LIS_TYPE_IMAGE_FORMAT /*!< Image format. See \ref lis_img_format . */
};

/*!
 * \brief Image format.
 *
 * Used to defined the type of content that will be returned by \ref lis_scan_session.scan_read() .
 */
enum lis_img_format {
	/*!
	 * \brief Raw image, 24bits per pixel.
	 *
	 * For each pixel:
	 * - 8bits for red,
	 * - 8bits for green,
	 * - 8bits for blue.
	 *
	 * No header, just pixels.
	 */
	LIS_IMG_FORMAT_RAW_RGB_24 = 0,

	LIS_IMG_FORMAT_GRAYSCALE_8,
	LIS_IMG_FORMAT_BW_1,
	LIS_IMG_FORMAT_BMP,
	LIS_IMG_FORMAT_GIF,
	LIS_IMG_FORMAT_JPEG,
	LIS_IMG_FORMAT_PNG,
};


enum lis_unit {
	LIS_UNIT_NONE = 0,
	LIS_UNIT_PIXEL,
	LIS_UNIT_BIT,
	LIS_UNIT_MM,
	LIS_UNIT_DPI,
	LIS_UNIT_PERCENT,
	LIS_UNIT_MICROSECOND,
};

/*!
 * \brief Variable.
 *
 * See \ref lis_value_type .
 */
union lis_value {
	int boolean;
	int integer;
	double dbl;
	const char *string;
	enum lis_img_format format;
};



struct lis_value_range {
	union lis_value min;
	union lis_value max;
	union lis_value interval;
};


struct lis_value_list {
	int nb_values;
	union lis_value *values;
};


/*!
 * \brief Describes a scanner or source option and provides callback to read or change its value.
 */
struct lis_option_descriptor {
	const char *name; /*!< option name / identifier (ex: "source", "resolution", etc). */
	const char *title; /*!< Human readable title (usually in English). */
	const char *desc; /*!< Human readable description (usually in English). */

#define LIS_CAP_EMULATED (1<<0) /*!< emulated option (either by LibInsane or backend) */
#define LIS_CAP_AUTOMATIC (1<<1) /*!< can be set automatically by the driver */
#define LIS_CAP_HW_SELECT (1<<2) /*!< can be set by a hardware switch */
#define LIS_CAP_SW_SELECT (1<<3) /*!< read/write ; can be set by software */
	/* TODO(Jflesch): Sane provides SANE_CAP_SOFT_DETECT ? oO */
/*!
 * \brief requires another option to be set to a specific value to become active
 */
#define LIS_CAP_INACTIVE (1<<4)

#define LIS_OPT_IS_READABLE(opt_desc) (!((opt_desc)->capabilities & LIS_CAP_INACTIVE))
#define LIS_OPT_IS_WRITABLE(opt_desc) ((opt_desc)->capabilities & LIS_CAP_SW_SELECT)

	/*!
	 * \brief Option capabilities.
	 *
	 * Bit fields:
	 * - \ref LIS_CAP_EMULATED
	 * - \ref LIS_CAP_AUTOMATIC
	 * - \ref LIS_CAP_HW_SELECT
	 * - \ref LIS_CAP_SW_SELECT
	 * - \ref LIS_CAP_INACTIVE
	 */
	int capabilities;

	/*!
	 * \brief Description of the type of values for this option.
	 */
	struct {
		enum lis_value_type type; /*!< Type of this option. */
		enum lis_unit unit; /*!< Unit of this value. Only useful for integers and float */
	} value;

	/*!
	 * \brief Restrictions defining the possible values for this option.
	 */
	struct {
		enum {
			LIS_CONSTRAINT_NONE, /*!< No constraint */
			LIS_CONSTRAINT_RANGE, /*!< Range of values (integers only) */
			LIS_CONSTRAINT_LIST, /*!< The structure 'possible' contains a list of values */
		} type; /*!< Type of constaint */

		/*!
		 * Content undefined if \ref lis_option_descriptor.type ==
		 * \ref LIS_CONSTRAINT_NONE.
		 */
		union {
			/*!
			 * If \ref lis_option_descriptor.type == \ref LIS_CONSTRAINT_RANGE.
			 */
			struct lis_value_range range;

			/*!
			 * If \ref lis_option_descriptor.type == \ref LIS_CONSTRAINT_LIST.
			 */
			struct lis_value_list list;
		} possible;
	} constraint;

	struct {
		/*!
		 * \brief return the option value.
		 *
		 * No need to free the value. However further calls to this library functions may
		 * modify the value. Depending on the backend and the driver, setting another value
		 * may also change this value. If you need to keep the value, you
		 * \ref lis_copy "should copy the value".
		 *
		 * \param[in] opt option for which we want the value.
		 * \param[out] value option value. Type is defined by
		 *		\ref lis_option_descriptor.type .
		 * \retval LIS_OK value successful read. See \ref LIS_IS_OK.
		 * \retval LIS_ACCESS_DENIED value cannot be read because the option is inactive.
		 *		\ref value may or may not be set. See \ref LIS_IS_ERROR.
		 * \retval LIS_IO_ERROR things are going the wrong way. \ref value may or may not
		 *		be set.
		 *		See \ref LIS_IS_ERROR.
		 */
		enum lis_error (*get_value)(struct lis_option_descriptor *self,
				union lis_value *value);

#define LIS_SET_FLAG_INEXACT (1<<0) /*!< value was set, but not specifically at the given value */
#define LIS_SET_FLAG_MUST_RELOAD_OPTIONS (1<<1) /*!< has changed values of other options */
#define LIS_SET_FLAG_MUST_RELOAD_PARAMS (1<<2) /*!< has changed scan parameters */

		/*!
		 * \brief set the option value.
		 *
		 * \param[in] opt option for which we want the value.
		 * \param[out] value option value. Type is defined by
		 *		\ref lis_option_descriptor.type.
		 * \param[out] set_flags (can be NULL). Indicates side effect of setting this value.
		 * \retval LIS_OK value has been successfully set. See \ref LIS_IS_OK.
		 * \retval LIS_APPROXIMATE_VALUE value has been successfully set, but value has been
		 *		approximated. See \ref LIS_IS_OK.
		 * \retval LIS_ACCESS_DENIED option is read-only. See \ref LIS_IS_ERROR.
		 * \retval LIS_INVALID_VALUE value doesn't match in type or doesn't fit in expected
		 *		constraints. See \ref LIS_IS_ERROR.
		 */
		enum lis_error (*set_value)(struct lis_option_descriptor *self,
				union lis_value value, int *set_flags);
	} fn; /*!< Functions to read and set the option value */
};

/*!
 * \brief Describes what will be returned when scanning.
 */
struct lis_scan_parameters {
	/*!
	 * \brief Image format.
	 * This value is guaranteed to be true when scanning.
	 */
	enum lis_img_format format;

	/*!
	 * \brief Width of the image in pixels.
	 * This value is guaranteed to be true when scanning
	 */
	int width;

	/*!
	 * \brief Height of the image in pixels.
	 * \warning This value is *not* guaranteed to be true when scanning.
	 */
	int height;

	/*!
	 * \brief Estimated image size in bytes.
	 * Can be used to pre-allocate memory.
	 * \warning This value is *not* guaranteed to be true when scanning.
	 */
	size_t image_size;
};


struct lis_scan_session {
	/*!
	 * \brief Indicates if we have reached the end of the feed
	 */
	int (*end_of_feed)(struct lis_scan_session *session);

	/*!
	 * \brief Indicates if we have reached the end of the page being currently scanned
	 * If an error must occur, it will occur on \ref scan_read(), including a possible
	 * LIS_ER_
	 */
	int (*end_of_page)(struct lis_scan_session *session);

	/*!
	 * \brief Read a chunk of the scanned image.
	 * \param[in] session Scan session.
	 * \param[out] out_buffer Image chunk.
	 * \param[in,out] out_buffer_size In: maximum number of bytes that can be put in out_buffer.
	 *  Out: number of bytes read. If the value is 0, it does *not* mean end of page or end of
	 *  file. It just means there was no data to ready yet.
	 * \warning You're advised to NOT use \ref LIS_IS_OK(). You can however use safely
	 *		\ref LIS_IS_ERROR().
	 * \warning You should manage every non-error return value carefully.
	 * \retval LIS_OK A chunk of the current page/image has been read.
	 * \retval LIS_CANCELLED Scan has been cancelled by \ref scan_cancel() or by hardware.
	 *		You can throw the image chunks you got away. Do not call again \ref scan_read().
	 *		Do not call \ref scan_read() or \ref scan_cancel() again.
	 * \retval LIS_END_OF_PAGE A whole page has been read. Call again \ref scan_read() to read
	 *		the next page.
	 * \retval LIS_END_OF_FEED A whole page feed has been read. Do not call \ref scan_cancel().
	 * \retval LIS_WARMING_UP Scanner is warming up. No data available yet. Keep calling
	 *		\ref scan_read() until there is.
	 */
	enum lis_error (*scan_read) (
		struct lis_scan_session *session, void *out_buffer, size_t *buffer_size
	);

	/*!
	 * \brief Cancel the current scan session.
	 *
	 * This function stops the current scan session and releases any resource related to the scan
	 * session.
	 */
	void (*cancel)(struct lis_scan_session *session);
};


struct lis_item {
	const char *name; /*!< Item name */

	enum {
		LIS_ITEM_DEVICE,
		LIS_ITEM_FLATBED,
		LIS_ITEM_ADF,
		LIS_ITEM_UNIDENTIFIED = -1,
	} type;

	/*!
	 * \brief Get item's children (usually scan sources).
	 *
	 * Without workarounds or normalizers:
	 * - Sane: will return an empty list.
	 * - WIA: will return device sources (Flatbed, Automatic Document Feeder, etc).
	 *
	 * \param[in] self Usually a scanner (see \ref lis_api.get_device()).
	 * \param[out] children Usually scanner sources. List will be NULL terminated.
	 * \retval LIS_OK children has been set to a valid array of items. See \ref LIS_IS_OK.
	 */
	enum lis_error (*get_children)(struct lis_item *self, struct lis_item ***children);

	/*!
	 * \brief Get item's options.
	 * \param[in] self Item from which we want the option list.
	 * \param[out] descs Option list. NULL terminated.
	 * \retval LIS_OK descs has been set.
	 */
	enum lis_error (*get_options)(
		struct lis_item *self, struct lis_option_descriptor ***descs
	);

	/*!
	 * \brief Returns a description of what will be returned when scanning.
	 *
	 * This is only an estimation. While the image format and the width of the
	 * image are certain, the height may actually vary. Application must handle the
	 * case where something different will be scanned (longer or shorter image).
	 *
	 * \param[in] self Item from which the scan will be done.
	 * \param[out] parameters Estimation of what will be scanned.
	 * \retval LIS_OK parameters has been set.
	 */
	enum lis_error (*get_scan_parameters)(
		struct lis_item *self, struct lis_scan_parameters *parameters
	);

	/*!
	 * \brief Starts a scan session.
	 * \warning This operation may take many seconds.
	 * \param[in] self Item from which to scan.
	 * \param[out] session Scan session.
	 * \retval LIS_OK Scan of the first page has started.
	 */
	enum lis_error (*scan_start)(struct lis_item *self, struct lis_scan_session **session);

	/*!
	 * \brief Close the access to a scanner.
	 * Will also be done automatically if you call \ref lis_api.cleanup().
	 */
	void (*close)(struct lis_item *self);
};

enum lis_device_locations {
	LIS_DEVICE_LOCATIONS_ANY = 0, /*!< local and remote */
	LIS_DEVICE_LOCATIONS_LOCAL_ONLY,
};

/*!
 * \brief LibInsane C API.
 *
 * Initialized as soon as you get it.
 */
struct lis_api {
	const char *base_name; /*!< "Sane", "WIA", "TWAIN", etc */

	/*!
	 * \brief cleanup the implementation.
	 *
	 * Must be called once you're done with the implementation instance. Otherwise you will leak
	 * file descriptors and memory.
	 *
	 * Exact meaning depends on the implementation (call to `sane_cleanup()`, stopping a thread,
	 * etc).
	 *
	 * \warning Do not use the impl pointer anymore after calling this function. It will probably be freed.
	 */
	void (*cleanup)(struct lis_api *impl);

	/*!
	 * \brief Look for paper eaters.
	 *
	 * If you already know the device identifier of the scanner you want to use, you do not need
	 * to call this function. You can call directly \ref get_device.
	 *
	 * \warning This operation may take many seconds.
	 * \param[in] local_only 1 if only local devices must be reported, 0 if remote devices must also be reported
	 * \param[out] dev_infos will point to a list of device descriptions, NULL terminated. Will be
	 *   invalidated/freed at the next call of \ref list_devices() or \ref cleanup.
	 * \retval LIS_OK dev_infos has been set to a list of devices. See \ref LIS_IS_OK.
	 */
	enum lis_error (*list_devices)(
		struct lis_api *impl, enum lis_device_locations, struct lis_device_descriptor ***dev_infos
	);

	/*!
	 * \brief Open the access to a scanner.
	 * \param[in] dev_id Device identifier. See \ref list_devices().
	 * \param[out] item Item representing the scanner.
	 * \warning This operation may take many seconds.
	 * \retval LIS_OK item has been set to ta valid list of items. List is NULL terminated.
	 *		You *must* use \ref lis_item.close() on it later. See \ref LIS_IS_OK.
	 * \retval LIS_ERR_DEVICE_BUSY Another process is already using this scanner. item may or may
	 *		not be modified. See \ref LIS_IS_ERROR.
	 * \retval LIS_ERR_IO_ERROR Didn't work but don't know why ... Item may or may not be modified.
	 *		See \ref LIS_IS_ERROR.
	 * \retval LIS_ERR_ACCESS_DENIED Permission denied. See \ref LIS_IS_ERROR.
	 */
	enum lis_error (*get_device)(struct lis_api *impl, const char *dev_id, struct lis_item **item);
};

#ifdef __cplusplus
}
#endif

#endif
