#include <libinsane/log.h>

#include <libinsane-gobject/error.h>
#include <libinsane-gobject/error_private.h>
#include <libinsane-gobject/scan_session.h>
#include <libinsane-gobject/scan_session_private.h>


struct _LibinsaneScanSessionPrivate
{
	struct lis_scan_session *session;
	int finished;
};
#define LIBINSANE_SCAN_SESSION_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), LIBINSANE_SCAN_SESSION_TYPE, \
	LibinsaneScanSessionPrivate))


static void libinsane_scan_session_finalize(GObject *self)
{
	LibinsaneScanSession *session = LIBINSANE_SCAN_SESSION(self);
	LibinsaneScanSessionPrivate *private = LIBINSANE_SCAN_SESSION_GET_PRIVATE(session);
	lis_log_debug("[gobject] Finalizing");
	if (!private->finished) {
		private->session->cancel(private->session);
		private->finished = 1;
	}
}


static void libinsane_scan_session_class_init(LibinsaneScanSessionClass *cls)
{
	GObjectClass *go_cls;
	g_type_class_add_private(cls, sizeof(LibinsaneScanSessionPrivate));
	go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_scan_session_finalize;
}


static void libinsane_scan_session_init(LibinsaneScanSession *self)
{
	lis_log_debug("[gobject] Initializing");
}

LibinsaneScanSession *libinsane_scan_session_new_from_libinsane(
		struct lis_scan_session *scan_session
	)
{
	LibinsaneScanSession *session;
	LibinsaneScanSessionPrivate *private;

	lis_log_debug("[gobject] enter");
	session = g_object_new(LIBINSANE_SCAN_SESSION_TYPE, NULL);
	private = LIBINSANE_SCAN_SESSION_GET_PRIVATE(session);
	private->session = scan_session;
	private->finished = 0;
	lis_log_debug("[gobject] leave");

	return session;
}

gboolean libinsane_scan_session_end_of_feed(LibinsaneScanSession *self)
{
	LibinsaneScanSessionPrivate *private = LIBINSANE_SCAN_SESSION_GET_PRIVATE(self);
	return private->session->end_of_feed(private->session) > 0;
}


gboolean libinsane_scan_session_end_of_page(LibinsaneScanSession *self)
{
	LibinsaneScanSessionPrivate *private = LIBINSANE_SCAN_SESSION_GET_PRIVATE(self);
	return private->session->end_of_page(private->session) > 0;
}


/**
 * libinsane_scan_session_read:
 * @self: Scan session
 * @buffer: (array length=lng) (element-type guint8): buffer to read data
 * @lng: length of buffer
 * @error: location to store the error if any occurs
 *
 * You must call libinsane_scan_session_end_of_feed() and libinsane_scan_session_end_of_page()
 * after each call to this function before calling it again.
 *
 * Returns: Number of bytes read, or -1 on error
 */
gssize libinsane_scan_session_read(LibinsaneScanSession *self, void *buffer, gsize lng, GError **error)
{
	LibinsaneScanSessionPrivate *private = LIBINSANE_SCAN_SESSION_GET_PRIVATE(self);
	size_t buf_length = lng;
	enum lis_error err;

	lis_log_debug("enter");
	err = private->session->scan_read(private->session, buffer, &buf_length);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane scan_session->read() error: 0x%X, %s",
			err, lis_strerror(err));
		lis_log_debug("error");
		return -1;
	}
	lis_log_debug("leave");
	return buf_length;
}


/**
 * libinsane_scan_session_read_bytes:
 * @self: scan session
 * @lng: number of bytes wanted
 * @error: set if an error occurs
 *
 * Returns: (transfer full): a new #GBytes, or %NULL if an error occured
 */
GBytes *libinsane_scan_session_read_bytes(LibinsaneScanSession *self, gsize lng, GError **error)
{
	guchar *buf;
	gssize nread;

	buf = g_malloc(lng);
	nread = libinsane_scan_session_read(self, buf, lng, error);
	if (nread < 0) {
		g_free(buf);
		return NULL;
	} else if (nread == 0) {
		g_free(buf);
		return g_bytes_new_static("", 0);
	} else {
		return g_bytes_new_take(buf, nread);
	}
}


void libinsane_scan_session_cancel(LibinsaneScanSession *self)
{
	LibinsaneScanSessionPrivate *private = LIBINSANE_SCAN_SESSION_GET_PRIVATE(self);
	private->session->cancel(private->session);
	private->finished = 1;
}


G_DEFINE_TYPE(LibinsaneScanSession, libinsane_scan_session, G_TYPE_OBJECT)
