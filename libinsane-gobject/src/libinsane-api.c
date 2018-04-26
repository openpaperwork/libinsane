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


LibinsaneApi *libinsane_api_new_safebet(GError **error)
{
	return g_object_new(LIBINSANE_API_TYPE, NULL);
}


LibinsaneApi *libinsane_api_new_from_string(const char *desc, GError **error)
{
	return g_object_new(LIBINSANE_API_TYPE, NULL);
}


G_DEFINE_TYPE(LibinsaneApi, libinsane_api, G_TYPE_OBJECT)
