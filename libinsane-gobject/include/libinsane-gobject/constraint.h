#ifndef __LIBINSANE_GOBJECT_CONSTRAINT_TYPE
#define __LIBINSANE_GOBJECT_CONSTRAINT_TYPE

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum
{
	LIBINSANE_CONSTRAINT_TYPE_NONE,
	LIBINSANE_CONSTRAINT_TYPE_RANGE,
	LIBINSANE_CONSTRAINT_TYPE_LIST,
} LibinsaneConstraintType;

G_END_DECLS

#define LIBINSANE_RANGE_TYPE \
     (libinsane_range_get_type())
#define LIBINSANE_RANGE(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_RANGE_TYPE, LibinsaneRange))
#define LIBINSANE_RANGE_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_RANGE_TYPE, LibinsaneRangeClass))
#define LIBINSANE_IS_RANGE(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_RANGE_TYPE))
#define LIBINSANE_RANGE_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), LIBINSANE_RANGE_TYPE, LibinsaneRangeClass))

typedef struct _LibinsaneRange LibinsaneRange;
typedef struct _LibinsaneRangePrivate LibinsaneRangePrivate;
typedef struct _LibinsaneRangeClass LibinsaneRangeClass;

struct _LibinsaneRange {
    GObject parent;
};

struct _LibinsaneRangeClass {
    GObjectClass parent;
};

GType libinsane_range_get_type(void) G_GNUC_CONST;

int libinsane_range_get_min(LibinsaneRange *self);
int libinsane_range_get_max(LibinsaneRange *self);
int libinsane_range_get_interval(LibinsaneRange *self);

#endif
