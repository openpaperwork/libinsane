#include <libinsane/capi.h>
#include <libinsane/log.h>

#include <libinsane-gobject/constraint.h>
#include <libinsane-gobject/option_descriptor.h>
#include <libinsane-gobject/img_format.h>
#include <libinsane-gobject/unit.h>

#include <enums.h>


struct _LibinsaneOptionDescriptorPrivate
{
	struct lis_option_descriptor *opt;
};

#define LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((obj), LIBINSANE_OPTION_DESCRIPTOR_TYPE, \
		LibinsaneOptionDescriptorPrivate))


static void libinsane_option_descriptor_finalize(GObject *object)
{
	lis_log_debug("[gobject] Finalizing");
}


static void libinsane_option_descriptor_class_init(LibinsaneOptionDescriptorClass *cls)
{
	GObjectClass *go_cls;
	g_type_class_add_private(cls, sizeof(LibinsaneOptionDescriptorPrivate));
	go_cls = G_OBJECT_CLASS(cls);
	go_cls->finalize = libinsane_option_descriptor_finalize;
}


static void libinsane_option_descriptor_init(LibinsaneOptionDescriptor *self)
{
	lis_log_debug("[gobject] Initializing");
}


LibinsaneOptionDescriptor *libinsane_option_descriptor_new_from_libinsane(
		struct lis_option_descriptor *lis_opt
	)
{
	LibinsaneOptionDescriptor *opt;
	LibinsaneOptionDescriptorPrivate *private;

	lis_log_debug("[gobject] enter");
	opt = g_object_new(LIBINSANE_OPTION_DESCRIPTOR_TYPE, NULL);
	private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(opt);
	private->opt = lis_opt;
	lis_log_debug("[gobject] leave");

	return opt;
}


const char *libinsane_option_descriptor_get_name(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	return private->opt->name;
}


const char *libinsane_option_descriptor_get_title(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	return private->opt->title;
}


const char *libinsane_option_descriptor_get_desc(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	return private->opt->desc;
}


LibinsaneCapability libinsane_option_descriptor_get_capabilities(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	int cap = private->opt->capabilities;
	LibinsaneCapability out = 0;

	if (cap & LIS_CAP_EMULATED) {
		out |= LIBINSANE_CAPABILITY_EMULATED;
	}
	if (cap & LIS_CAP_AUTOMATIC) {
		out |= LIBINSANE_CAPABILITY_AUTOMATIC;
	}
	if (cap & LIS_CAP_HW_SELECT) {
		out |= LIBINSANE_CAPABILITY_HW_SELECT;
	}
	if (cap & LIS_CAP_SW_SELECT) {
		out |= LIBINSANE_CAPABILITY_SW_SELECT;
	}
	return out;
}


GType libinsane_option_descriptor_get_value_type(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);

	switch(private->opt->value.type) {
		case LIS_TYPE_BOOL:
			return G_TYPE_BOOLEAN;
		case LIS_TYPE_INTEGER:
			return G_TYPE_INT;
		case LIS_TYPE_DOUBLE:
			return G_TYPE_DOUBLE;
		case LIS_TYPE_STRING:
			return G_TYPE_STRING;
		case LIS_TYPE_IMAGE_FORMAT:
			return LIBINSANE_TYPE_IMG_FORMAT;
	}

	return G_TYPE_INVALID;
}


LibinsaneUnit libinsane_option_descriptor_get_value_unit(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	switch(private->opt->value.unit) {
		case LIS_UNIT_NONE:
			return LIBINSANE_UNIT_NONE;
		case LIS_UNIT_PIXEL:
			return LIBINSANE_UNIT_PIXEL;
		case LIS_UNIT_BIT:
			return LIBINSANE_UNIT_BIT;
		case LIS_UNIT_MM:
			return LIBINSANE_UNIT_MM;
		case LIS_UNIT_DPI:
			return LIBINSANE_UNIT_DPI;
		case LIS_UNIT_PERCENT:
			return LIBINSANE_UNIT_PERCENT;
		case LIS_UNIT_MICROSECOND:
			return LIBINSANE_UNIT_MICROSECOND;
	}
	return -1;
}


LibinsaneConstraintType libinsane_option_descriptor_get_constraint_type(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	switch(private->opt->constraint.type) {
		case LIS_CONSTRAINT_NONE:
			return LIBINSANE_CONSTRAINT_TYPE_NONE;
		case LIS_CONSTRAINT_RANGE:
			return LIBINSANE_CONSTRAINT_TYPE_RANGE;
		case LIS_CONSTRAINT_LIST:
			return LIBINSANE_CONSTRAINT_TYPE_LIST;
	}
	return -1;
}

static LibinsaneImgFormat lis_format_to_gobject_format(enum lis_img_format fmt)
{
	switch(fmt) {
		case LIS_IMG_FORMAT_RAW_RGB_24:
			return LIBINSANE_IMG_FORMAT_RAW_RGB_24;
		case LIS_IMG_FORMAT_GRAYSCALE_8:
			return LIBINSANE_IMG_FORMAT_GRAYSCALE_8;
		case LIS_IMG_FORMAT_BW_1:
			return LIBINSANE_IMG_FORMAT_BW_1;
		case LIS_IMG_FORMAT_BMP:
			return LIBINSANE_IMG_FORMAT_BMP;
		case LIS_IMG_FORMAT_GIF:
			return LIBINSANE_IMG_FORMAT_GIF;
		case LIS_IMG_FORMAT_JPEG:
			return LIBINSANE_IMG_FORMAT_JPEG;
		case LIS_IMG_FORMAT_PNG:
			return LIBINSANE_IMG_FORMAT_PNG;
	}
	return LIS_ENUM_ERROR;
}


static GValue lis_value_to_gvalue(enum lis_value_type type, union lis_value value)
{
	GValue out = G_VALUE_INIT;

	switch(type) {
		case LIS_TYPE_BOOL:
			g_value_init(&out, G_TYPE_BOOLEAN);
			g_value_set_boolean(&out, value.boolean > 0);
			break;
		case LIS_TYPE_INTEGER:
			g_value_init(&out, G_TYPE_INT);
			g_value_set_int(&out, value.integer);
			break;
		case LIS_TYPE_DOUBLE:
			g_value_init(&out, G_TYPE_DOUBLE);
			g_value_set_double(&out, value.dbl);
			break;
		case LIS_TYPE_STRING:
			g_value_init(&out, G_TYPE_STRING);
			g_value_set_static_string(&out, value.string);
			break;
		case LIS_TYPE_IMAGE_FORMAT:
			g_value_init(&out, LIBINSANE_TYPE_IMG_FORMAT);
			g_value_set_enum(&out, lis_format_to_gobject_format(value.format));
			break;
	}
	return out;
}

/**
 * libinsane_option_descriptor_get_constraint:
 * @self option for which we want the constraint
 *
 * Returns: (element-type GValue) (transfer container): item scan options.
 */
GArray *libinsane_option_descriptor_get_constraint(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	GArray *out = NULL;
	GValue val = G_VALUE_INIT;
	int i;

	switch(private->opt->constraint.type) {
		case LIS_CONSTRAINT_NONE:
			return NULL;
		case LIS_CONSTRAINT_RANGE:
			out = g_array_sized_new(FALSE /* !zero terminated */,
					TRUE /* cleared to 0 when allocated */,
					sizeof(GValue),
					3 /* nb elements */);

			val = lis_value_to_gvalue(private->opt->value.type,
					private->opt->constraint.possible.range.min);
			out = g_array_append_val(out, val);

			val = lis_value_to_gvalue(private->opt->value.type,
					private->opt->constraint.possible.range.max);
			out = g_array_append_val(out, val);

			val = lis_value_to_gvalue(private->opt->value.type,
					private->opt->constraint.possible.range.interval);
			out = g_array_append_val(out, val);

			return out;

		case LIS_CONSTRAINT_LIST:
			out = g_array_sized_new(FALSE /* !zero terminated */,
					TRUE /* cleared to 0 when allocated */,
					sizeof(GValue),
					/* nb elements */
					private->opt->constraint.possible.list.nb_values);
			for (i = 0 ; i < private->opt->constraint.possible.list.nb_values ; i++) {
				val = lis_value_to_gvalue(private->opt->value.type,
						private->opt->constraint.possible.list.values[i]);
				out = g_array_append_val(out, val);
			}
			return out;
	}

	return NULL;
}


GValue libinsane_option_descriptor_get_value(LibinsaneOptionDescriptor *self, GError **error)
{
	GValue val = G_VALUE_INIT;
	/* TODO */
	g_value_init(&val, G_TYPE_INT);
	g_value_set_int(&val, 0);
	return val;
}


void libinsane_option_descriptor_set_value(LibinsaneOptionDescriptor *self, GValue *value, GError **error)
{
	/* TODO */
}


G_DEFINE_TYPE(LibinsaneOptionDescriptor, libinsane_option_descriptor, G_TYPE_OBJECT)
