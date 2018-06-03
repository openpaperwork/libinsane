#include <assert.h>
#include <stdlib.h>
#include <strings.h>

#include <libinsane/log.h>
#include <libinsane/safebet.h>

#include <libinsane-gobject/device_descriptor.h>
#include <libinsane-gobject/device_descriptor_private.h>
#include <libinsane-gobject/error.h>
#include <libinsane-gobject/error_private.h>
#include <libinsane-gobject/img_format.h>
#include <libinsane-gobject/item.h>
#include <libinsane-gobject/item_private.h>
#include <libinsane-gobject/libinsane-api.h>

#include <enums.h>


struct _LibinsaneApiPrivate
{
	struct lis_api *impl;
};

#define LIBINSANE_API_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), LIBINSANE_API_TYPE, LibinsaneApiPrivate))


static void libinsane_api_finalize(GObject *self)
{
	lis_log_debug("Finalizing");
	libinsane_api_cleanup(LIBINSANE_API(self));
}


static void str2img_format(const GValue *src_value, GValue *dest_value)
{
	const char *str;
	LibinsaneImgFormat format;

	assert(G_VALUE_HOLDS_STRING(src_value));

	str = g_value_get_string(src_value);

	if (strcasecmp(str, "raw24") == 0 || strcasecmp(str, "raw") == 0) {
		format = LIBINSANE_IMG_FORMAT_RAW_RGB_24;
	} else if (strcasecmp(str, "raw_grayscale") == 0) {
		format = LIBINSANE_IMG_FORMAT_GRAYSCALE_8;
	} else if (strcasecmp(str, "raw_bw") == 0) {
		format = LIBINSANE_IMG_FORMAT_BW_1;
	} else if (strcasecmp(str, "bmp") == 0) {
		format = LIBINSANE_IMG_FORMAT_BMP;
	} else if (strcasecmp(str, "gif") == 0) {
		format = LIBINSANE_IMG_FORMAT_GIF;
	} else if (strcasecmp(str, "jpeg") == 0 || strcasecmp(str, "jpg") == 0) {
		format = LIBINSANE_IMG_FORMAT_JPEG;
	} else if (strcasecmp(str, "png") == 0) {
		format = LIBINSANE_IMG_FORMAT_PNG;
	} else {
		lis_log_warning("Unknown image format '%s'. Assuming raw RGB 24bits", str);
		format = LIBINSANE_IMG_FORMAT_RAW_RGB_24;
	}

	g_value_set_enum(dest_value, format);
}


static void libinsane_api_class_init(LibinsaneApiClass *cls)
{
	GObjectClass *go_cls;

	g_type_class_add_private(cls, sizeof(LibinsaneApiPrivate));
	go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_api_finalize;

	g_value_register_transform_func(G_TYPE_STRING, LIBINSANE_TYPE_IMG_FORMAT, str2img_format);
}


static void libinsane_api_init(LibinsaneApi *self)
{
	lis_log_debug("Initializing");
}


/**
 * libinsane_api_new_safebet:
 *
 * Instantiate the safest list of API for most cases.
 * See [lis_safebet](../doxygen/html/safebet_8h.html)
 *
 * Returns: (transfer full): the new reference to @self.
 */
LibinsaneApi *libinsane_api_new_safebet(GError **error)
{
	LibinsaneApi *api = g_object_new(LIBINSANE_API_TYPE, NULL);
	LibinsaneApiPrivate *priv = LIBINSANE_API_GET_PRIVATE(api);

	enum lis_error err;

	lis_log_debug("enter");

	err = lis_safebet(&priv->impl);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane init error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("error");
		return NULL;
	}
	assert(priv->impl != NULL);
	lis_log_debug("leave");

	return api;
}


/**
 * libinsane_api_new_from_string:
 *
 * Instantiate a new list of API based on a provided string.
 *
 * @param desc see [lis_str2impls()](../doxygen/html/str2impls_8h.html)
 *
 * Returns: (transfer full): the new reference to @self.
 */
LibinsaneApi *libinsane_api_new_from_string(const char *desc, GError **error)
{
	LibinsaneApi *impl;
	lis_log_debug("enter");
	impl = g_object_new(LIBINSANE_API_TYPE, NULL);
	lis_log_debug("leave");
	return impl;
}



void libinsane_api_cleanup(LibinsaneApi *self)
{
	LibinsaneApiPrivate *priv;

	lis_log_debug("enter");

	priv = LIBINSANE_API_GET_PRIVATE(self);
	if (priv->impl != NULL) {
		lis_log_debug("cleanup");
		priv->impl->cleanup(priv->impl);
		priv->impl = NULL;
	}

	lis_log_debug("leave");
}


/**
 * libinsane_api_list_devices:
 *
 * Returns: (element-type Libinsane.DeviceDescriptor) (transfer full):
 *   list of available devices (LibinsaneItem objects)
 */
GList *libinsane_api_list_devices(
	LibinsaneApi *self, LibinsaneDeviceLocations locations, GError **error
)
{
	LibinsaneApiPrivate *priv;
	enum lis_error err;
	struct lis_device_descriptor **dev_infos;
	LibinsaneDeviceDescriptor *dev_info;
	GList *out = NULL;
	int i;
	enum lis_device_locations lis_locations = LIS_DEVICE_LOCATIONS_ANY;

	lis_log_debug("enter");

	switch(locations) {
		case LIBINSANE_DEVICE_LOCATIONS_ANY:
			lis_locations = LIS_DEVICE_LOCATIONS_ANY;
			break;
		case LIBINSANE_DEVICE_LOCATIONS_LOCAL_ONLY:
			lis_locations = LIS_DEVICE_LOCATIONS_LOCAL_ONLY;
			break;
	}

	priv = LIBINSANE_API_GET_PRIVATE(self);
	err = priv->impl->list_devices(priv->impl, lis_locations, &dev_infos);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane get devices error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("error");
		return NULL;
	}

	for (i = 0 ; dev_infos[i] != NULL ; i++) {
		dev_info = libinsane_device_descriptor_new_from_libinsane(dev_infos[i]);
		out = g_list_prepend(out, dev_info);
	}

	lis_log_debug("leave");
	return g_list_reverse(out);
}


/**
 * libinsane_api_get_device:
 *
 * Returns: (transfer none): list of available devices (LibinsaneItem objects)
 */
LibinsaneItem *libinsane_api_get_device(LibinsaneApi *self, const char *dev_id, GError **error) {
	LibinsaneApiPrivate *priv;
	struct lis_item *lis_item;
	LibinsaneItem *item;
	enum lis_error lis_err;

	lis_log_debug("enter");

	priv = LIBINSANE_API_GET_PRIVATE(self);

	lis_err = priv->impl->get_device(priv->impl, dev_id, &lis_item);
	if (LIS_IS_ERROR(lis_err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, lis_err,
			"Libinsane get devices error: 0x%X, %s",
			lis_err, lis_strerror(lis_err));
		lis_log_debug("error");
		return NULL;
	}

	item = libinsane_item_new_from_libinsane(lis_item);
	lis_log_debug("leave");
	return item;
}

G_DEFINE_TYPE(LibinsaneApi, libinsane_api, G_TYPE_OBJECT)
