#include <libinsane/capi.h>
#include <libinsane/log.h>

#include <libinsane-gobject/item.h>


struct _LibinsaneItemPrivate
{
	struct lis_item *item;
};

#define LIBINSANE_ITEM_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), LIBINSANE_ITEM_TYPE, LibinsaneItemPrivate))


static void libinsane_item_finalize(GObject *object)
{
	lis_log_debug("[gobject] Finalizing");
}


static void libinsane_item_class_init(LibinsaneItemClass *cls)
{
	GObjectClass *go_cls;
	g_type_class_add_private(cls, sizeof(LibinsaneItemPrivate));
	go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_item_finalize;
}


static void libinsane_item_init(LibinsaneItem *self)
{
	lis_log_debug("[gobject] Initializing");
}


LibinsaneItem *libinsane_item_new_from_libinsane(struct lis_item *lis_item)
{
	LibinsaneItem *item;
	LibinsaneItemPrivate *private;

	lis_log_debug("[gobject] enter");
	item = g_object_new(LIBINSANE_ITEM_TYPE, NULL);
	private = LIBINSANE_ITEM_GET_PRIVATE(item);
	private->item = lis_item;
	lis_log_debug("[gobject] leave");

	return item;
}

/**
 * libinsane_item_get_name:
 * @self: current item
 *
 * Returns: (transfer none): Item's name
 */
const char *libinsane_item_get_name(LibinsaneItem *self)
{
	LibinsaneItemPrivate *private;

	private = LIBINSANE_ITEM_GET_PRIVATE(self);
	return private->item->name;
}


LibinsaneItemType libinsane_item_get_item_type(LibinsaneItem *self)
{
	return 0; /* TODO */
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
