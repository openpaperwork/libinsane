#include <assert.h>
#include <stdlib.h>

#include <libinsane/log.h>
#include <libinsane/safebet.h>

#include <libinsane-gobject/device_descriptor.h>
#include <libinsane-gobject/device_descriptor_private.h>
#include <libinsane-gobject/error.h>
#include <libinsane-gobject/error_private.h>
#include <libinsane-gobject/libinsane-api.h>

struct _LibinsaneApiPrivate
{
	struct lis_api *impl;
};

#define LIBINSANE_API_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), LIBINSANE_API_TYPE, LibinsaneApiPrivate))


static void libinsane_api_finalize(GObject *self)
{
	lis_log_debug("[gobject] Finalizing");
	libinsane_api_cleanup(LIBINSANE_API(self));
}


static void libinsane_api_class_init(LibinsaneApiClass *cls)
{
	GObjectClass *go_cls;

	g_type_class_add_private(cls, sizeof(LibinsaneApiPrivate));
	go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_api_finalize;
}


static void libinsane_api_init(LibinsaneApi *self)
{
	lis_log_debug("[gobject] Initializing");
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

	lis_log_debug("[gobject] enter");

	err = lis_safebet(&priv->impl);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane init error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("[gobject] error");
		return NULL;
	}
	assert(priv->impl != NULL);
	lis_log_debug("[gobject] leave");

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
	lis_log_debug("[gobject] enter");
	impl = g_object_new(LIBINSANE_API_TYPE, NULL);
	lis_log_debug("[gobject] leave");
	return impl;
}



void libinsane_api_cleanup(LibinsaneApi *self)
{
	LibinsaneApiPrivate *priv;

	lis_log_debug("[gobject] enter");

	priv = LIBINSANE_API_GET_PRIVATE(self);
	if (priv->impl != NULL) {
		lis_log_debug("[gobject] cleanup");
		priv->impl->cleanup(priv->impl);
		priv->impl = NULL;
	}

	lis_log_debug("[gobject] leave");
}


/**
 * libinsane_api_get_devices:
 *
 * Returns: (element-type Libinsane.DeviceDescriptor) (transfer full):
 *   list of available devices (LibinsaneItem objects)
 */
GList *libinsane_api_get_devices(LibinsaneApi *self, gboolean local_only, GError **error) {
	LibinsaneApiPrivate *priv;
	enum lis_error err;
	struct lis_device_descriptor **dev_infos;
	LibinsaneDeviceDescriptor *dev_info;
	GList *out = NULL;
	int i;

	lis_log_debug("[gobject] enter");

	priv = LIBINSANE_API_GET_PRIVATE(self);
	err = priv->impl->get_devices(priv->impl, local_only, &dev_infos);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane get devices error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("[gobject] error");
		return NULL;
	}

	for (i = 0 ; dev_infos[i] != NULL ; i++) {
		dev_info = libinsane_device_descriptor_new_from_libinsane(dev_infos[i]);
		out = g_list_prepend(out, dev_info);
	}

	lis_log_debug("[gobject] leave");
	return g_list_reverse(out);
}


/**
 * libinsane_api_dev_get:
 *
 * Returns: (transfer none): list of available devices (LibinsaneItem objects)
 */
LibinsaneItem *libinsane_api_dev_get(LibinsaneApi *self, const char *dev_id) {
	return NULL; /* TODO */
}

G_DEFINE_TYPE(LibinsaneApi, libinsane_api, G_TYPE_OBJECT)
