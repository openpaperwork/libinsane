#include <libinsane-gobject/scan_session.h>


static void libinsane_scan_session_finalize(GObject *object)
{

}


static void libinsane_scan_session_class_init(LibinsaneScanSessionClass *cls)
{
	GObjectClass *go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_scan_session_finalize;
}


static void libinsane_scan_session_init(LibinsaneScanSession *self)
{

}


G_DEFINE_TYPE(LibinsaneScanSession, libinsane_scan_session, G_TYPE_OBJECT)
