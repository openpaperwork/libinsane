#ifndef __LIBINSANE_GOBJECT_SCAN_SESSION_H
#define __LIBINSANE_GOBJECT_SCAN_SESSION_H

#include <gio/gio.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define LIBINSANE_SCAN_SESSION_TYPE \
     (libinsane_scan_session_get_type())
#define LIBINSANE_SCAN_SESSION(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), LIBINSANE_SCAN_SESSION_TYPE, LibinsaneScanSession))
#define LIBINSANE_SCAN_SESSION_CLASS(cls) \
    (G_TYPE_CHECK_CLASS_CAST ((cls), LIBINSANE_SCAN_SESSION_TYPE, LibinsaneScanSessionClass))
#define LIBINSANE_IS_SCAN_SESSION(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LIBINSANE_SCAN_SESSION_TYPE))
#define LIBINSANE_SCAN_SESSION_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), LIBINSANE_SCAN_SESSION_TYPE, LibinsaneScanSessionClass))

typedef struct _LibinsaneScanSession LibinsaneScanSession;
typedef struct _LibinsaneScanSessionPrivate LibinsaneScanSessionPrivate;
typedef struct _LibinsaneScanSessionClass LibinsaneScanSessionClass;

struct _LibinsaneScanSession {
    GInputStream parent;
};

struct _LibinsaneScanSessionClass {
    GInputStreamClass parent;
};

GType libinsane_scan_session_get_type(void) G_GNUC_CONST;

/* no public constructor */

gboolean libinsane_scan_session_end_of_feed(LibinsaneScanSession *self);
gboolean libinsane_scan_session_end_of_page(LibinsaneScanSession *self);
gssize libinsane_scan_session_read(LibinsaneScanSession *self, void *buffer, gsize lng, GError **error);
GBytes *libinsane_scan_session_read_bytes(LibinsaneScanSession *self, gsize lng, GError **error);
void libinsane_scan_session_cancel(LibinsaneScanSession *self);

G_END_DECLS

#endif
