#include <libinsane-gobject/item.h>


static void libinsane_item_finalize(GObject *object)
{

}


static void libinsane_item_class_init(LibinsaneItemClass *cls)
{
	GObjectClass *go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_item_finalize;
}


static void libinsane_item_init(LibinsaneItem *self)
{

}


G_DEFINE_TYPE(LibinsaneItem, libinsane_item, G_TYPE_OBJECT)
