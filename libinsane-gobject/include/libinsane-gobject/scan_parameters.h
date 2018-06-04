#ifndef __LIBINSANE_GOBJECT_SCAN_PARAMETERS_H
#define __LIBINSANE_GOBJECT_SCAN_PARAMETERS_H

#include <glib-object.h>

#include "img_format.h"

G_BEGIN_DECLS

#define LIBINSANE_SCAN_PARAMETERS_TYPE \
     (libinsane_scan_parameters_get_type())
#define LIBINSANE_SCAN_PARAMETERS(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_SCAN_PARAMETERS_TYPE, LibinsaneScanParameters))
#define LIBINSANE_SCAN_PARAMETERS_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_SCAN_PARAMETERS_TYPE, LibinsaneScanParametersClass))
#define LIBINSANE_IS_SCAN_PARAMETERS(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_SCAN_PARAMETERS_TYPE))
#define LIBINSANE_SCAN_PARAMETERS_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), LIBINSANE_SCAN_PARAMETERS_TYPE, LibinsaneScanParametersClass))

typedef struct _LibinsaneScanParameters LibinsaneScanParameters;
typedef struct _LibinsaneScanParametersPrivate LibinsaneScanParametersPrivate;
typedef struct _LibinsaneScanParametersClass LibinsaneScanParametersClass;

struct _LibinsaneScanParameters {
    GObject parent;
};

struct _LibinsaneScanParametersClass {
    GObjectClass parent;
};

GType libinsane_scan_parameters_get_type(void) G_GNUC_CONST;

/* no public constructor */

LibinsaneImgFormat libinsane_scan_parameters_get_format(LibinsaneScanParameters *self);

int libinsane_scan_parameters_get_width(LibinsaneScanParameters *self);
int libinsane_scan_parameters_get_height(LibinsaneScanParameters *self);
gsize libinsane_scan_parameters_get_image_size(LibinsaneScanParameters *self);

G_END_DECLS

#endif
