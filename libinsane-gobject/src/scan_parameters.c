#include <libinsane-gobject/scan_parameters.h>


static void libinsane_scan_parameters_finalize(GObject *object)
{

}


static void libinsane_scan_parameters_class_init(LibinsaneScanParametersClass *cls)
{
	GObjectClass *go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_scan_parameters_finalize;
}


static void libinsane_scan_parameters_init(LibinsaneScanParameters *self)
{

}


G_DEFINE_TYPE(LibinsaneScanParameters, libinsane_scan_parameters, G_TYPE_OBJECT)
