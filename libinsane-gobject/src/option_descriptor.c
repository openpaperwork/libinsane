#include <assert.h>
#include <stdio.h>

#include <libinsane/capi.h>
#include <libinsane/log.h>

#include <libinsane-gobject/constraint.h>
#include <libinsane-gobject/error.h>
#include <libinsane-gobject/error_private.h>
#include <libinsane-gobject/option_descriptor.h>
#include <libinsane-gobject/img_format.h>
#include <libinsane-gobject/unit.h>

#include <enums.h>


struct _LibinsaneOptionDescriptorPrivate
{
	struct lis_option_descriptor *opt;
	GValue last_value;
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

	/* set a default 'last_value' ; won't be actually used ; just here to be freed */
	g_value_init(&private->last_value, G_TYPE_INT);
	g_value_set_int(&private->last_value, 0);

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


static void lis_value_to_gvalue(enum lis_value_type type, union lis_value value, GValue *out)
{
	g_value_unset(out);
	switch(type) {
		case LIS_TYPE_BOOL:
			g_value_init(out, G_TYPE_BOOLEAN);
			g_value_set_boolean(out, value.boolean > 0);
			return;
		case LIS_TYPE_INTEGER:
			g_value_init(out, G_TYPE_INT);
			g_value_set_int(out, value.integer);
			return;
		case LIS_TYPE_DOUBLE:
			g_value_init(out, G_TYPE_DOUBLE);
			g_value_set_double(out, value.dbl);
			return;
		case LIS_TYPE_STRING:
			g_value_init(out, G_TYPE_STRING);
			g_value_set_static_string(out, value.string);
			return;
		case LIS_TYPE_IMAGE_FORMAT:
			g_value_init(out, LIBINSANE_TYPE_IMG_FORMAT);
			g_value_set_enum(out, lis_format_to_gobject_format(value.format));
			return;
	}
	lis_log_error("Unknown value type: %d", type);
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

			lis_value_to_gvalue(private->opt->value.type,
				private->opt->constraint.possible.range.min,
				&val);
			out = g_array_append_val(out, val);

			lis_value_to_gvalue(private->opt->value.type,
				private->opt->constraint.possible.range.max,
				&val);
			out = g_array_append_val(out, val);

			lis_value_to_gvalue(private->opt->value.type,
				private->opt->constraint.possible.range.interval,
				&val);
			out = g_array_append_val(out, val);

			return out;

		case LIS_CONSTRAINT_LIST:
			out = g_array_sized_new(FALSE /* !zero terminated */,
					TRUE /* cleared to 0 when allocated */,
					sizeof(GValue),
					/* nb elements */
					private->opt->constraint.possible.list.nb_values);
			for (i = 0 ; i < private->opt->constraint.possible.list.nb_values ; i++) {
				lis_value_to_gvalue(private->opt->value.type,
					private->opt->constraint.possible.list.values[i],
					&val);
				out = g_array_append_val(out, val);
			}
			return out;
	}

	return NULL;
}


gboolean libinsane_option_descriptor_is_readable(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	return LIS_OPT_IS_READABLE(private->opt);
}


gboolean libinsane_option_descriptor_is_writable(LibinsaneOptionDescriptor *self)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	return LIS_OPT_IS_WRITABLE(private->opt);
}


const GValue *libinsane_option_descriptor_get_value(LibinsaneOptionDescriptor *self, GError **error)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	enum lis_error err;
	union lis_value val;

	lis_log_debug("enter");
	err = private->opt->fn.get_value(private->opt, &val);
	if (LIS_IS_ERROR(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane opt[%s]->get_value() error: 0x%X, %s",
			private->opt->name, err, lis_strerror(err));
		lis_log_debug("error");
		return NULL;
	}

	lis_value_to_gvalue(private->opt->value.type, val, &private->last_value);
	lis_log_debug("enter");
	return &private->last_value;
}


static int gvalue_to_lis_value(const GValue *in, enum lis_value_type type, union lis_value *out)
{
	GValue converted = G_VALUE_INIT;

	switch(type) {
		case LIS_TYPE_BOOL:
			g_value_init(&converted, G_TYPE_BOOLEAN);
			if (!g_value_transform(in, &converted)) {
				lis_log_error("Failed to convert value to boolean");
				return 0;
			}
			out->boolean = g_value_get_boolean(&converted);
			return 1;
		case LIS_TYPE_INTEGER:
			g_value_init(&converted, G_TYPE_INT);
			if (!g_value_transform(in, &converted)) {
				lis_log_error("Failed to convert value to integer");
				return 0;
			}
			out->integer = g_value_get_int(&converted);
			return 1;
		case LIS_TYPE_DOUBLE:
			g_value_init(&converted, G_TYPE_DOUBLE);
			if (!g_value_transform(in, &converted)) {
				lis_log_error("Failed to convert value to double");
				return 0;
			}
			out->dbl = g_value_get_double(&converted);
			return 1;
		case LIS_TYPE_STRING:
			g_value_init(&converted, G_TYPE_STRING);
			g_value_transform(in, &converted);
			if (!g_value_transform(in, &converted)) {
				lis_log_error("Failed to convert value to string");
				return 0;
			}
			out->string = g_value_get_string(&converted);
			return 1;
		case LIS_TYPE_IMAGE_FORMAT:
			g_value_init(&converted, LIBINSANE_TYPE_IMG_FORMAT);
			g_value_transform(in, &converted);
			if (!g_value_transform(in, &converted)) {
				lis_log_error("Failed to convert value to string");
				return 0;
			}
			out->format = g_value_get_enum(&converted);
			return 1;
	}

	assert(0);
	lis_log_error("Unknown type: %d", type);
	return 0;
}


LibinsaneSetFlag libinsane_option_descriptor_set_value(LibinsaneOptionDescriptor *self,
		GValue *value, GError **error
	)
{
	LibinsaneOptionDescriptorPrivate *private = LIBINSANE_OPTION_DESCRIPTOR_GET_PRIVATE(self);
	enum lis_error err;
	union lis_value val;
	int set_flags = 0;
	LibinsaneSetFlag out;

	lis_log_debug("enter");
	if (!gvalue_to_lis_value(value, private->opt->value.type, &val)) {
		err = LIS_ERR_INVALID_VALUE;
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane opt[%s]->set_value() conversion error: 0x%X, %s",
			private->opt->name, err, lis_strerror(err));
		lis_log_debug("conversion error");
		return LIBINSANE_SET_FLAG_NONE;
	}

	err = private->opt->fn.set_value(private->opt, val, &set_flags);
	if (!LIS_IS_OK(err)) {
		SET_LIBINSANE_GOBJECT_ERROR(error, err,
			"Libinsane opt[%s]->set_value() error: 0x%X, %s",
			private->opt->name, err, lis_strerror(err));
		lis_log_debug("error");
		return LIBINSANE_SET_FLAG_NONE;
	}

	out = LIBINSANE_SET_FLAG_NONE;
	if (set_flags & LIS_SET_FLAG_INEXACT) {
		out |= LIBINSANE_SET_FLAG_INEXACT;
	}
	if (set_flags & LIS_SET_FLAG_MUST_RELOAD_OPTIONS) {
		out |= LIBINSANE_SET_FLAG_MUST_RELOAD_OPTIONS;
	}
	if (set_flags & LIS_SET_FLAG_MUST_RELOAD_PARAMS) {
		out |= LIBINSANE_SET_FLAG_MUST_RELOAD_PARAMS;
	}

	lis_log_debug("leave");
	return out;
}


G_DEFINE_TYPE(LibinsaneOptionDescriptor, libinsane_option_descriptor, G_TYPE_OBJECT)
