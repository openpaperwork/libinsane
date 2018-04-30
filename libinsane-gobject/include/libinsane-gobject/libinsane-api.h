#ifndef __LIBINSANE_GOBJECT_API_H
#define __LIBINSANE_GOBJECT_API_H

#include <glib-object.h>

#include "item.h"

G_BEGIN_DECLS

#define LIBINSANE_API_TYPE \
     (libinsane_api_get_type())
#define LIBINSANE_API(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_API_TYPE, LibinsaneApi))
#define LIBINSANE_API_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_API_TYPE, LibinsaneApiClass))
#define LIBINSANE_IS_API(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_API_TYPE))
#define LIBINSANE_IS_API_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_TYPE ((cls),  LIBINSANE_API_TYPE))
#define LIBINSANE_API_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), LIBINSANE_API_TYPE, LibinsaneApiClass))

typedef struct _LibinsaneApi LibinsaneApi;
typedef struct _LibinsaneApiPrivate LibinsaneApiPrivate;
typedef struct _LibinsaneApiClass LibinsaneApiClass;

struct _LibinsaneApi {
    GObject parent;
};

struct _LibinsaneApiClass {
    GObjectClass parent;
};

GType libinsane_api_get_type(void) G_GNUC_CONST;

/**
 * libinsane_api_new_safebet:
 * Instantiate the safest list of API for most cases.
 * See \ref lis_safebet()
 *
 * Returns: (transfer full): the new reference to @self.
 */
LibinsaneApi *libinsane_api_new_safebet(GError **error);

/**
 * libinsane_api_new_from_string:
 * Instantiate a new list of API based on a provided string.
 *
 * @param desc see \ref lis_str2impls()
 *
 * Returns: (transfer full): the new reference to @self.
 */
LibinsaneApi *libinsane_api_new_from_string(const char *desc, GError **error);


void libinsane_api_cleanup(LibinsaneApi *self); /* TODO */


/**
 * libinsane_api_get_devices:
 * Returns: (transfer full): list of available devices (LibinsaneItem objects)
 */
GValueArray *libinsane_api_get_devices(LibinsaneApi *self, GError **error); /* TODO */


/**
 * libinsane_api_dev_get:
 * Returns: (transfer none): list of available devices (LibinsaneItem objects)
 */
LibinsaneItem *libinsane_api_dev_get(LibinsaneApi *self, const char *dev_id); /* TODO */


G_END_DECLS

#endif
