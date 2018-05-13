#ifndef __LIBINSANE_GOBJECT_DEVICE_DESCRIPTOR_H
#define __LIBINSANE_GOBJECT_DEVICE_DESCRIPTOR_H

#include <glib-object.h>

G_BEGIN_DECLS

#define LIBINSANE_DEVICE_DESCRIPTOR_TYPE \
     (libinsane_device_descriptor_get_type())
#define LIBINSANE_DEVICE_DESCRIPTOR(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_DEVICE_DESCRIPTOR_TYPE, LibinsaneDeviceDescriptor))
#define LIBINSANE_DEVICE_DESCRIPTOR_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_DEVICE_DESCRIPTOR_TYPE, LibinsaneDeviceDescriptorClass))
#define LIBINSANE_IS_DEVICE_DESCRIPTOR(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_DEVICE_DESCRIPTOR_TYPE))
#define LIBINSANE_DEVICE_DESCRIPTOR_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), LIBINSANE_DEVICE_DESCRIPTOR_TYPE, LibinsaneDeviceDescriptorClass))

typedef struct _LibinsaneDeviceDescriptor LibinsaneDeviceDescriptor;
typedef struct _LibinsaneDeviceDescriptorPrivate LibinsaneDeviceDescriptorPrivate;
typedef struct _LibinsaneDeviceDescriptorClass LibinsaneDeviceDescriptorClass;

struct _LibinsaneDeviceDescriptor {
    GObject parent;
};

struct _LibinsaneDeviceDescriptorClass {
    GObjectClass parent;
};

GType libinsane_device_descriptor_get_type(void) G_GNUC_CONST;

/* no public constructor */

const char *libinsane_device_descriptor_get_api(LibinsaneDeviceDescriptor *self);
const char *libinsane_device_descriptor_get_dev_id(LibinsaneDeviceDescriptor *self);
const char *libinsane_device_descriptor_get_dev_vendor(LibinsaneDeviceDescriptor *self);
const char *libinsane_device_descriptor_get_dev_model(LibinsaneDeviceDescriptor *self);
const char *libinsane_device_descriptor_get_dev_type(LibinsaneDeviceDescriptor *self);
const char *libinsane_device_descriptor_to_string(LibinsaneDeviceDescriptor *self);

G_END_DECLS

#endif
