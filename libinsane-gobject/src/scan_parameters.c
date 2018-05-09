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

int libinsane_scan_parameters_get_format(LibinsaneScanParameters *self)
{
	return 0; /* TODO */
}

int libinsane_scan_parameters_get_width(LibinsaneScanParameters *self)
{
	return 0; /* TODO */
}

int libinsane_scan_parameters_get_height(LibinsaneScanParameters *self)
{
	return 0; /* TODO */
}

int libinsane_scan_parameters_get_image_size(LibinsaneScanParameters *self)
{
	return 0; /* TODO */
}

G_DEFINE_TYPE(LibinsaneScanParameters, libinsane_scan_parameters, G_TYPE_OBJECT)
