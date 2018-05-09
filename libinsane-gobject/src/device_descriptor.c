#include <libinsane-gobject/device_descriptor.h>


static void libinsane_device_descriptor_finalize(GObject *object)
{

}


static void libinsane_device_descriptor_class_init(LibinsaneDeviceDescriptorClass *cls)
{
	GObjectClass *go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_device_descriptor_finalize;
}


static void libinsane_device_descriptor_init(LibinsaneDeviceDescriptor *self)
{

}

const char *libinsane_device_descriptor_get_api_name(LibinsaneDeviceDescriptor *self)
{
	return NULL; /* TODO */
}


const char *libinsane_device_descriptor_get_dev_id(LibinsaneDeviceDescriptor *self)
{
	return NULL; /* TODO */
}


const char *libinsane_device_descriptor_get_dev_vendor(LibinsaneDeviceDescriptor *self)
{
	return NULL; /* TODO */
}


const char *libinsane_device_descriptor_get_dev_model(LibinsaneDeviceDescriptor *self)
{
	return NULL; /* TODO */
}


const char *libinsane_device_descriptor_get_dev_type(LibinsaneDeviceDescriptor *self)
{
	return NULL; /* TODO */
}


G_DEFINE_TYPE(LibinsaneDeviceDescriptor, libinsane_device_descriptor, G_TYPE_OBJECT)
