#include <libinsane-gobject/libinsane-api.h>


static void libinsane_api_finalize(GObject *object)
{

}


static void libinsane_api_class_init(LibinsaneApiClass *cls)
{
	GObjectClass *go_cls = G_OBJECT_CLASS(cls);
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
	return g_object_new(LIBINSANE_API_TYPE, NULL);
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
	/* TODO */
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
