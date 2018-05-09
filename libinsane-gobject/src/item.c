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

void libinsane_item_open(LibinsaneItem *self, GError **error)
{
	/* TODO */
}


void libinsane_item_close(LibinsaneItem *self, GError **error)
{
	/* TODO */
}


/**
 * libinsane_item_get_children:
 * @self: current item. Should be a root item (not a result #libinsane_item_get_children()),
 *     not a children.
 * @error: in case an error occurred while looking for children items
 *
 * See [C-API](../doxygen/html/structlis__item.html#ae4039acefa6acf85a110dec491340411)
 *
 * Returns: (transfer full): list of children items (usually scan sources)
 */
GValueArray *libinsane_item_get_children(LibinsaneItem *self, GError **error)
{
	return NULL; /* TODO */
}


/**
 * libinsane_item_get_options:
 * @self: current item. Can be either a root or child item.
 * @error: in case an error occurred while looking for options.
 *
 * See [C-API](../doxygen/html/structlis__item.html#aa2e301604accfe64461b36e28365bf9e)
 *
 * Returns: (transfer full): item scan options.
 */
GValueArray *libinsane_item_get_options(LibinsaneItem *self, GError **error)
{
	return NULL; /* TODO */
}


/**
 * libinsane_item_get_scan_parameters:
 * Returns: (transfer full): item scan parameters.
 */
LibinsaneScanParameters *libinsane_item_get_scan_parameters(LibinsaneItem *self, GError **error)
{
	return NULL; /* TODO */
}


/**
 * libinsane_item_scan_start:
 * Returns: (transfer full): item scan parameters.
 */
LibinsaneScanSession *libinsane_item_scan_start(LibinsaneItem *self, GError **error)
{
	return NULL; /* TODO */
}


G_DEFINE_TYPE(LibinsaneItem, libinsane_item, G_TYPE_OBJECT)
