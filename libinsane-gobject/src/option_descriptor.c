#include <libinsane-gobject/option_descriptor.h>


static void libinsane_option_descriptor_finalize(GObject *object)
{

}


static void libinsane_option_descriptor_class_init(LibinsaneOptionDescriptorClass *cls)
{
	GObjectClass *go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_option_descriptor_finalize;
}


static void libinsane_option_descriptor_init(LibinsaneOptionDescriptor *self)
{

}


const char *libinsane_option_descriptor_get_name(LibinsaneOptionDescriptor *self)
{
	return NULL; /* TODO */
}


const char *libinsane_option_descriptor_get_title(LibinsaneOptionDescriptor *self)
{
	return NULL; /* TODO */
}


const char *libinsane_option_descriptor_get_desc(LibinsaneOptionDescriptor *self)
{
	return NULL; /* TODO */
}


LibinsaneCapability libinsane_option_descriptor_get_capabilities(LibinsaneOptionDescriptor *self)
{
	return 0; /* TODO */
}


GType libinsane_option_descriptor_get_value_type(LibinsaneOptionDescriptor *self)
{
	return 0; /* TODO */
}


LibinsaneUnit libinsane_option_descriptor_get_value_unit(LibinsaneOptionDescriptor *self)
{
	return 0; /* TODO */
}


GValue *libinsane_option_descriptor_get_constraint(LibinsaneOptionDescriptor *self)
{
	return NULL; /* TODO */
}


GValue *libinsane_option_get_value(LibinsaneOptionDescriptor *self, GError **error)
{
	return NULL; /* TODO */
}


void libinsane_option_set_value(LibinsaneOptionDescriptor *self, GValue *value, GError **error)
{
	/* TODO */
}


G_DEFINE_TYPE(LibinsaneOptionDescriptor, libinsane_option_descriptor, G_TYPE_OBJECT)
