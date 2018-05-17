#include <libinsane/log.h>

#include <libinsane-gobject/device_descriptor.h>
#include <libinsane-gobject/device_descriptor_private.h>


struct _LibinsaneDeviceDescriptorPrivate
{
	char *api;
	char *dev_id;
	char *vendor;
	char *model;
	char *type;
	char *to_string;
};

#define LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), LIBINSANE_DEVICE_DESCRIPTOR_TYPE, LibinsaneDeviceDescriptorPrivate))



static void libinsane_device_descriptor_finalize(GObject *object)
{
	LibinsaneDeviceDescriptor *self = LIBINSANE_DEVICE_DESCRIPTOR(object);
	LibinsaneDeviceDescriptorPrivate *private = LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(self);

	lis_log_debug("[gobject] Finalizing");
	g_free(private->api);
	g_free(private->dev_id);
	g_free(private->vendor);
	g_free(private->model);
	g_free(private->type);
	g_free(private->to_string);
}


static void libinsane_device_descriptor_class_init(LibinsaneDeviceDescriptorClass *cls)
{
	GObjectClass *go_cls;
	g_type_class_add_private(cls, sizeof(LibinsaneDeviceDescriptorPrivate));
	go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_device_descriptor_finalize;
}


static void libinsane_device_descriptor_init(LibinsaneDeviceDescriptor *self)
{
	lis_log_debug("[gobject] Initializing");
}

LibinsaneDeviceDescriptor *libinsane_device_descriptor_new_from_libinsane(
		const struct lis_device_descriptor *lis_desc
	)
{
	LibinsaneDeviceDescriptor *desc;
	LibinsaneDeviceDescriptorPrivate *private;

	lis_log_debug("[gobject] enter");
	desc = g_object_new(LIBINSANE_DEVICE_DESCRIPTOR_TYPE, NULL);
	private = LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(desc);
	private->api = g_strdup(lis_desc->impl->base_name);
	private->dev_id = g_strdup(lis_desc->dev_id);
	private->vendor = g_strdup(lis_desc->vendor);
	private->model = g_strdup(lis_desc->model);
	private->type = g_strdup(lis_desc->type);
	private->to_string = g_strdup_printf(
		"%s %s (%s ; %s ; %s)",
		private->vendor, private->model, private->type, private->api, private->dev_id
	);
	lis_log_debug("[gobject] leave");
	return desc;
}


const char *libinsane_device_descriptor_get_api(LibinsaneDeviceDescriptor *self)
{
	LibinsaneDeviceDescriptorPrivate *private = LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(self);
	return private->api;
}


const char *libinsane_device_descriptor_get_dev_id(LibinsaneDeviceDescriptor *self)
{
	LibinsaneDeviceDescriptorPrivate *private = LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(self);
	return private->dev_id;
}


const char *libinsane_device_descriptor_get_dev_vendor(LibinsaneDeviceDescriptor *self)
{
	LibinsaneDeviceDescriptorPrivate *private = LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(self);
	return private->vendor;
}


const char *libinsane_device_descriptor_get_dev_model(LibinsaneDeviceDescriptor *self)
{
	LibinsaneDeviceDescriptorPrivate *private = LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(self);
	return private->model;
}


const char *libinsane_device_descriptor_get_dev_type(LibinsaneDeviceDescriptor *self)
{
	LibinsaneDeviceDescriptorPrivate *private = LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(self);
	return private->type;
}

/**
 * libinsane_device_descriptor_to_string:
 * Convenience method: allow to see quickly which device is designated by this object.
 * Do not use in production code.
 */
const char *libinsane_device_descriptor_to_string(LibinsaneDeviceDescriptor *self)
{
	LibinsaneDeviceDescriptorPrivate *private = LIBINSANE_DEVICE_DESCRIPTOR_GET_PRIVATE(self);
	return private->to_string;
}


G_DEFINE_TYPE(LibinsaneDeviceDescriptor, libinsane_device_descriptor, G_TYPE_OBJECT)
