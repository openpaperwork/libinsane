#ifndef __LIBINSANE_GOBJECT_H
#define __LIBINSANE_GOBJECT_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define LIBINSANE_API_TYPE                \
     (libinsane_api_get_type())
#define LIBINSANE_API(o)                  \
    (G_TYPE_CHECK_INSTANCE_CAST ((o), LIBINSANE_API_TYPE, LibinsaneApi))
#define LIBINSANE_CLASS(c)            \
    (G_TYPE_CHECK_CLASS_CAST ((c), LIBINSANE_API_TYPE, LibinsaneApiClass))
#define LIBINSANE_IS_API(o)               \
    (G_TYPE_CHECK_INSTANCE_TYPE ((o), LIBINSANE_API_TYPE))
#define LIBINSANE_IS_API_CLASS(c)         \
    (G_TYPE_CHECK_CLASS_TYPE ((c),  LIBINSANE_API_TYPE))
#define LIBINSANE_API_GET_CLASS(o)        \
    (G_TYPE_INSTANCE_GET_CLASS ((o), LIBINSANE_API_TYPE, LibinsaneApiClass))

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

G_END_DECLS

#endif
