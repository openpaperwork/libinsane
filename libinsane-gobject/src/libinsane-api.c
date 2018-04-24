#include <libinsane-gobject/libinsane-api.h>


static void libinsane_api_document_finalize(GObject *object)
{

}


static void libinsane_api_class_init(LibinsaneApiClass *cls)
{
	GObjectClass *go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_api_document_finalize;
}


static void libinsane_api_init(LibinsaneApi *self)
{

}


LibinsaneApi *libinsane_api_new_from_string(const char *desc)
{
	return g_object_new(LIBINSANE_API_TYPE, NULL);
}


G_DEFINE_TYPE(LibinsaneApi, libinsane_api, G_TYPE_OBJECT)
