#include <libinsane/capi.h>
#include <libinsane/log.h>

#include <libinsane-gobject/error.h>
#include <libinsane-gobject/error_private.h>
#include <libinsane-gobject/item.h>
#include <libinsane-gobject/option_descriptor.h>
#include <libinsane-gobject/option_descriptor_private.h>
#include <libinsane-gobject/scan_parameters.h>
#include <libinsane-gobject/scan_parameters_private.h>
#include <libinsane-gobject/scan_session.h>
#include <libinsane-gobject/scan_session_private.h>


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
	LibinsaneItemPrivate *private = LIBINSANE_ITEM_GET_PRIVATE(self);
	return private->item->name;
}


LibinsaneItemType libinsane_item_get_item_type(LibinsaneItem *self)
{
	LibinsaneItemPrivate *private = LIBINSANE_ITEM_GET_PRIVATE(self);
	switch(private->item->type)
	{
		case LIS_ITEM_DEVICE:
			return LIBINSANE_ITEM_TYPE_DEVICE;
		case LIS_ITEM_FLATBED:
			return LIBINSANE_ITEM_TYPE_FLATBED;
		case LIS_ITEM_ADF:
			return LIBINSANE_ITEM_TYPE_ADF;
		case LIS_ITEM_UNIDENTIFIED:
			return LIBINSANE_ITEM_TYPE_UNIDENTIFIED;
	}

	lis_log_warning("Unexpected item type: %d", private->item->type);
	return LIBINSANE_ITEM_TYPE_UNIDENTIFIED;
}


void libinsane_item_close(LibinsaneItem *self, GError **error)
{
	LibinsaneItemPrivate *private = LIBINSANE_ITEM_GET_PRIVATE(self);
	private->item->close(private->item);
}


/**
 * libinsane_item_get_children:
 * @self: current item. Should be a root item (not a result #libinsane_item_get_children()),
 *     not a children.
 * @error: in case an error occurred while looking for children items
 *
 * See [C-API](../doxygen/html/structlis__item.html#ae4039acefa6acf85a110dec491340411)
 *
 * Returns: (element-type Libinsane.Item) (transfer full):
 *   list of children items (usually scan sources)
 */
GList *libinsane_item_get_children(LibinsaneItem *self, GError **error)
{
	LibinsaneItemPrivate *private = LIBINSANE_ITEM_GET_PRIVATE(self);
	struct lis_item **children;
	GList *out = NULL;
	enum lis_error err;
	LibinsaneItem *item;
	int i;

	lis_log_debug("enter");
	err = private->item->get_children(private->item, &children);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane item->get_children() error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("error");
		return NULL;
	}

	for (i = 0 ; children[i] != NULL ; i++) {
		item = libinsane_item_new_from_libinsane(children[i]);
		out = g_list_prepend(out, item);
	}

	lis_log_debug("leave");
	return g_list_reverse(out);
}


/**
 * libinsane_item_get_options:
 * @self: current item. Can be either a root or child item.
 * @error: in case an error occurred while looking for options.
 *
 * See [C-API](../doxygen/html/structlis__item.html#aa2e301604accfe64461b36e28365bf9e)
 *
 * Returns: (element-type Libinsane.OptionDescriptor) (transfer full): item scan options.
 */
GList *libinsane_item_get_options(LibinsaneItem *self, GError **error)
{
	LibinsaneItemPrivate *private = LIBINSANE_ITEM_GET_PRIVATE(self);
	enum lis_error err;
	struct lis_option_descriptor **opts;
	LibinsaneOptionDescriptor *desc;
	int i;
	GList *out = NULL;

	lis_log_debug("enter");
	err = private->item->get_options(private->item, &opts);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane item->get_options() error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("error");
		return NULL;
	}

	for (i = 0 ; opts[i] != NULL ; i++) {
		desc = libinsane_option_descriptor_new_from_libinsane(opts[i]);
		out = g_list_prepend(out, desc);
	}

	lis_log_debug("leave");
	return g_list_reverse(out);
}


/**
 * libinsane_item_get_scan_parameters:
 * Returns: (transfer full): item scan parameters.
 */
LibinsaneScanParameters *libinsane_item_get_scan_parameters(LibinsaneItem *self, GError **error)
{
	LibinsaneItemPrivate *private = LIBINSANE_ITEM_GET_PRIVATE(self);
	struct lis_scan_parameters lis_params;
	enum lis_error err;
	LibinsaneScanParameters *params;

	lis_log_debug("enter");
	err = private->item->get_scan_parameters(private->item, &lis_params);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane item->get_scan_parameters() error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("error");
		return NULL;
	}

	params = libinsane_scan_parameters_new_from_libinsane(&lis_params);
	lis_log_debug("leave");

	return params;
}


/**
 * libinsane_item_scan_start:
 * Returns: (transfer full): item scan parameters.
 */
LibinsaneScanSession *libinsane_item_scan_start(LibinsaneItem *self, GError **error)
{
	LibinsaneItemPrivate *private = LIBINSANE_ITEM_GET_PRIVATE(self);
	enum lis_error err;
	struct lis_scan_session *lis_scan_session = NULL;
	LibinsaneScanSession *scan_session;

	lis_log_debug("enter");
	err = private->item->scan_start(private->item, &lis_scan_session);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane item->scan_start() error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("error");
		return NULL;
	}

	scan_session = libinsane_scan_session_new_from_libinsane(lis_scan_session);
	lis_log_debug("leave");
	return scan_session;
}


G_DEFINE_TYPE(LibinsaneItem, libinsane_item, G_TYPE_OBJECT)
