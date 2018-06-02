#ifndef __LIBINSANE_GOBJECT_OPTION_DESCRIPTOR_H
#define __LIBINSANE_GOBJECT_OPTION_DESCRIPTOR_H

#include <glib-object.h>

#include "capability.h"
#include "constraint.h"
#include "unit.h"

G_BEGIN_DECLS

typedef enum {
    LIBINSANE_SET_FLAG_NONE = 0,
    LIBINSANE_SET_FLAG_INEXACT = (1<<0),
    LIBINSANE_SET_FLAG_MUST_RELOAD_OPTIONS = (1<<1),
    LIBINSANE_SET_FLAG_MUST_RELOAD_PARAMS = (1<<2),
} LibinsaneSetFlag;

#define LIBINSANE_OPTION_DESCRIPTOR_TYPE \
     (libinsane_option_descriptor_get_type())
#define LIBINSANE_OPTION_DESCRIPTOR(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_OPTION_DESCRIPTOR_TYPE, LibinsaneOptionDescriptor))
#define LIBINSANE_OPTION_DESCRIPTOR_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_OPTION_DESCRIPTOR_TYPE, LibinsaneOptionDescriptorClass))
#define LIBINSANE_IS_OPTION_DESCRIPTOR(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_OPTION_DESCRIPTOR_TYPE))
#define LIBINSANE_OPTION_DESCRIPTOR_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), LIBINSANE_OPTION_DESCRIPTOR_TYPE, LibinsaneOptionDescriptorClass))

typedef struct _LibinsaneOptionDescriptor LibinsaneOptionDescriptor;
typedef struct _LibinsaneOptionDescriptorPrivate LibinsaneOptionDescriptorPrivate;
typedef struct _LibinsaneOptionDescriptorClass LibinsaneOptionDescriptorClass;

struct _LibinsaneOptionDescriptor {
    GObject parent;
};

struct _LibinsaneOptionDescriptorClass {
    GObjectClass parent;
};

GType libinsane_option_descriptor_get_type(void) G_GNUC_CONST;

const char *libinsane_option_descriptor_get_name(LibinsaneOptionDescriptor *self);
const char *libinsane_option_descriptor_get_title(LibinsaneOptionDescriptor *self);
const char *libinsane_option_descriptor_get_desc(LibinsaneOptionDescriptor *self);
LibinsaneCapability libinsane_option_descriptor_get_capabilities(LibinsaneOptionDescriptor *self);
GType libinsane_option_descriptor_get_value_type(LibinsaneOptionDescriptor *self);
LibinsaneUnit libinsane_option_descriptor_get_value_unit(LibinsaneOptionDescriptor *self);
LibinsaneConstraintType libinsane_option_descriptor_get_constraint_type(LibinsaneOptionDescriptor *self);
GArray *libinsane_option_descriptor_get_constraint(LibinsaneOptionDescriptor *self);
gboolean libinsane_option_descriptor_is_readable(LibinsaneOptionDescriptor *self);
gboolean libinsane_option_descriptor_is_writable(LibinsaneOptionDescriptor *self);

const GValue *libinsane_option_descriptor_get_value(LibinsaneOptionDescriptor *self, GError **error);
LibinsaneSetFlag libinsane_option_descriptor_set_value(LibinsaneOptionDescriptor *self, GValue *value, GError **error);

G_END_DECLS

#endif
