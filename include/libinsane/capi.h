#ifndef __LIBINSANE_CAPI_H
#define __LIBINSANE_CAPI_H

#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

struct lis_device_description {
	const char *api_name; /*!< "Sane", "WIA", "TWAIN", etc */

	const char *dev_id;

	const char *vendor;
	const char *model;
	const char *type;
};

enum lis_value_type {
		enum {
			LIS_TYPE_BOOL, /*!< int: 0 (false) or 1 (true)*/
			LIS_TYPE_INTEGER, /*!< int */
			LIS_TYPE_FLOAT, /*!< float */
			LIS_TYPE_STRING, /*!< (char *) */
		} type;
};

union lis_value {
	int boolean;
	int integer;
	float flt;
	const char *string;
};

enum lis_format {
	LIS_FORMAT_RGB_24,

	LIS_FORMAT_GRAYSCALE_8,
	LIS_FORMAT_BW_1,
	LIS_FORMAT_BMP,
	LIS_FORMAT_GIF,
	LIS_FORMAT_JPEG,
	LIS_FORMAT_PNG,
};

struct lis_option_description {
	const char *name;
	const char *title;
	const char *desc;

#define LIS_CAP_EMULATED (1<<0) /*!< emulated option (either by LibInsane or backend) */
#define LIS_CAP_AUTOMATIC (1<<1) /*!< can be set automatically by the driver */
#define LIS_CAP_HW_SELECT (1<<2) /*!< can be set by a hardware switch */
#define LIS_CAP_SW_SELECT (1<<3) /*!< read/write ; can be set by software */
	/* TODO(Jflesch): Sane provides SANE_CAP_SOFT_DETECT ? oO */
#define LIS_CAP_INACTIVE (1<<4) /*!< requires another option to be set to a specific value to become active */
	int capabilities;

	struct {
		enum lis_value_type type;

		enum {
			LIS_UNIT_NONE,
			LIS_UNIT_PIXEL,
			LIS_UNIT_BIT,
			LIS_UNIT_MM,
			LIS_UNIT_DPI,
			LIS_UNIT_PERCENT,
			LIS_UNIT_MICROSECOND,
		} unit;
	} value;

	struct {
		enum {
			LIS_CONSTRAINT_NONE, /*!< no constraint */
			LIS_CONSTRAINT_INTEGER_RANGE, /*!< the structure 'possible' contains a range of integers */
			LIS_CONSTRAINT_INTEGER_LIST, /*!< the structure 'possible' contains a list of integers */
			LIS_CONSTRAINT_STRING_LIST, /*!< the structure 'possible' contains a list of strings */
			LIS_CONSTRAINT_IMG_FORMAT, /*!< the structure 'possible' contains a list of possible image formats */
		} type;

		union {
			struct {
				int min;
				int max;
				int interval;
			} range;
			const char **strings;
			struct {
				int *values;
				int nb_values;
			} integers;
			struct {
				enum lis_format *formats;
				int nb_formats;
			} formats;
		} possible;
	} constraint;

	struct {
		enum lis_error (*get_value)(struct lis_option_description *opt, union lis_value **value);
		enum lis_error (*set_value)(struct lis_option_description *opt, const union lis_value *value);
	} fn;
};

struct lis_scan_parameters {
	enum lis_format format;
	int width;
	int height;
};

struct lis_api {
	void (*init)(struct lis_api *api);
	void (*cleanup)(struct lis_api *api);

	enum lis_error (*get_devices)(
		struct lis_api *api, struct lis_device_description ***dev_infos, int nb_devices
	);

	enum lis_error (*open)(const char *dev_id, struct lis_item **item);
	void *(*close)(struct lis_item *dev);

	enum lis_error (*get_children)(struct lis_item *item, struct lis_item ***children);

	enum lis_error (*get_options)(struct lis_item *item, struct lis_option_description **descs);
	enum lis_error (*reload_option_description)(struct lis_item *item, struct lis_option_description *desc);
	enum lis_error (*get_scan_parameters)(struct list_item *item, struct lis_scan_parameters **parameters);

	enum lis_error (*start)(struct lis_item *item, struct lis_scan_session **session);
	enum lis_error (*read)(struct lis_scan_session *session, void *out_buffer, size_t out_buffer_size);
	enum lis_error (*cancel)(struct lis_scan_session *session);
};

#ifdef __cplusplus
}
#endif

#endif
