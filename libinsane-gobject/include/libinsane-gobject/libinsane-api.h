#ifndef __LIBINSANE_GOBJECT_API_H
#define __LIBINSANE_GOBJECT_API_H

#include <glib-object.h>

#include "item.h"

G_BEGIN_DECLS

typedef enum {
	LIBINSANE_DEVICE_LOCATIONS_ANY = 0,
	LIBINSANE_DEVICE_LOCATIONS_LOCAL_ONLY,
} LibinsaneDeviceLocations;

#define LIBINSANE_API_TYPE \
     (libinsane_api_get_type())
#define LIBINSANE_API(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_API_TYPE, LibinsaneApi))
#define LIBINSANE_API_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_API_TYPE, LibinsaneApiClass))
#define LIBINSANE_IS_API(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_API_TYPE))
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

LibinsaneApi *libinsane_api_new_safebet(GError **error);
LibinsaneApi *libinsane_api_new_from_string(const char *desc, GError **error);
GList *libinsane_api_list_devices(
	LibinsaneApi *self, LibinsaneDeviceLocations locations, GError **error
);
LibinsaneItem *libinsane_api_get_device(LibinsaneApi *self, const char *dev_id, GError **error);
void libinsane_api_cleanup(LibinsaneApi *self);

G_END_DECLS

#endif
