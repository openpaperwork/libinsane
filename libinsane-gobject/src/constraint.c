#include <libinsane-gobject/constraint.h>


static void libinsane_range_finalize(GObject *object)
{

}


static void libinsane_range_class_init(LibinsaneRangeClass *cls)
{
	GObjectClass *go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_range_finalize;
}


static void libinsane_range_init(LibinsaneRange *self)
{

}

int libinsane_range_get_min(LibinsaneRange *self)
{
	return 0; /* TODO */
}


int libinsane_range_get_max(LibinsaneRange *self)
{
	return 0; /* TODO */
}


int libinsane_range_get_interval(LibinsaneRange *self)
{
	return 0; /* TODO */
}


G_DEFINE_TYPE(LibinsaneRange, libinsane_range, G_TYPE_OBJECT)
