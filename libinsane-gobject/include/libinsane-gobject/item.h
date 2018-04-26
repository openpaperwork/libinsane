#ifndef __LIBINSANE_GOBJECT_ITEM_H
#define __LIBINSANE_GOBJECT_ITEM_H

#include <glib-object.h>

G_BEGIN_DECLS

#define libinsane_item_TYPE \
     (libinsane_item_get_type())
#define libinsane_item(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_ITEM_TYPE, LibinsaneItem))
#define libinsane_item_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_ITEM_TYPE, LibinsaneItemClass))
#define LIBINSANE_IS_ITEM(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_ITEM_TYPE))
#define LIBINSANE_IS_ITEM_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_TYPE ((cls),  LIBINSANE_ITEM_TYPE))
#define LIBINSANE_ITEM_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), LIBINSANE_ITEM_TYPE, LibinsaneItemClass))

typedef struct _LibinsaneItem LibinsaneItem;
typedef struct _LibinsaneItemPrivate LibinsaneItemPrivate;
typedef struct _LibinsaneItemClass LibinsaneItemClass;

struct _LibinsaneItem {
    GObject parent;
};

struct _LibinsaneItemClass {
    GObjectClass parent;
};

GType libinsane_item_get_type(void) G_GNUC_CONST;

/* no public constructor */

void libinsane_item_open(LibinsaneItem *item, GError **error); /* TODO */
void libinsane_item_close(LibinsaneItem *item, GError **error); /* TODO */

/**
 * Returns: (transfer full): list of children items (usually scan sources)
 */
GValueArray libinsane_item_get_children(LibinsaneItem *item, GError **error); /* TODO */


/**
 * Returns: (transfer full): item scan options.
 */
GValueArray libinsane_item_get_options(LibinsaneItem *item, GError **error); /* TODO */

/* xxx libinsane_item_get_scan_parameters(LibinsaneItem *item, GError **error); */ /* TODO */

/* xxx libinsane_item_scan_start(LibinsaneItem *item, GError **error); */ /* TODO */

#endif
