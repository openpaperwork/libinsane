#ifndef __LIBINSANE_GOBJECT_ITEM_H
#define __LIBINSANE_GOBJECT_ITEM_H

#include <glib-object.h>

#include "item_type.h"
#include "scan_parameters.h"
#include "scan_session.h"

G_BEGIN_DECLS

#define LIBINSANE_ITEM_TYPE \
     (libinsane_item_get_type())
#define LIBINSANE_ITEM(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_ITEM_TYPE, LibinsaneItem))
#define LIBINSANE_ITEM_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_ITEM_TYPE, LibinsaneItemClass))
#define LIBINSANE_IS_ITEM(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_ITEM_TYPE))
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

const char *libinsane_item_get_name(LibinsaneItem *self);
LibinsaneItemType libinsane_item_get_item_type(LibinsaneItem *self);
void libinsane_item_close(LibinsaneItem *self, GError **error);
GList *libinsane_item_get_children(LibinsaneItem *self, GError **error);
GList *libinsane_item_get_options(LibinsaneItem *self, GError **error); /* TODO */
LibinsaneScanParameters *libinsane_item_get_scan_parameters(LibinsaneItem *self, GError **error);
LibinsaneScanSession *libinsane_item_scan_start(LibinsaneItem *self, GError **error); /* TODO */

G_END_DECLS

#endif
