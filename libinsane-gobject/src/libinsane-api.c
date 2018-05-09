#include <libinsane/safebet.h>

#include <libinsane-gobject/error.h>
#include <libinsane-gobject/libinsane-api.h>

typedef struct _LibinsaneApiPrivate
{
	struct lis_api *impl;
} LibinsaneApiPrivate;

#define LIBINSANE_API_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), LIBINSANE_API_TYPE, LibinsaneApiPrivate))


static void libinsane_api_finalize(GObject *self)
{
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

	enum lis_error err = lis_safebet(&priv->impl);
	if (LIS_IS_ERROR(err)) {
		g_set_error(error, LIBINSANE_ERROR, err,
			"Libinsane init error: %d, %s",
			err, lis_strerror(err));
		return NULL;
	}

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
	return g_object_new(LIBINSANE_API_TYPE, NULL);
}



void libinsane_api_cleanup(LibinsaneApi *self)
{
	LibinsaneApiPrivate *priv = LIBINSANE_API_GET_PRIVATE(self);
	if (priv->impl != NULL) {
		priv->impl->cleanup(priv->impl);
		priv->impl = NULL;
	}
}


/**
 * libinsane_api_get_devices:
 *
 * Returns: (transfer full): list of available devices (LibinsaneItem objects)
 */
GValueArray *libinsane_api_get_devices(LibinsaneApi *self, GError **error) {
	return NULL; /* TODO */
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
