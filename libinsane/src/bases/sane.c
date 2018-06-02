#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <sane/sane.h>
#include <sane/saneopts.h>

#include <libinsane/capi.h>
#include <libinsane/log.h>
#include <libinsane/sane.h>
#include <libinsane/util.h>

#define API_NAME "sane"


struct lis_sane
{
	struct lis_api parent;
	struct lis_device_descriptor **dev_descs;
};
#define LIS_SANE_PRIVATE(impl) ((struct lis_sane *)(impl))

struct lis_sane_option
{
	struct lis_option_descriptor parent;
	int opt_idx;
	size_t value_size;
	char *value_buf;
	struct lis_sane_item *item;
};
#define LIS_SANE_OPTION(impl) ((struct lis_sane_option *)(impl))

struct lis_sane_item
{
	struct lis_item parent;
	SANE_Handle handle;

	int nb_opts;
	struct lis_sane_option *options;
	/*!< pointer array pointing to elements of options */
	struct lis_option_descriptor **option_ptrs;
};
#define LIS_SANE_ITEM_PRIVATE(impl) ((struct lis_sane_item *)(impl))

/* root functions */
static void lis_sane_cleanup(struct lis_api *impl);
static enum lis_error lis_sane_list_devices(struct lis_api *impl,
		enum lis_device_locations locations,
		struct lis_device_descriptor ***out_dev_descs);
static enum lis_error lis_sane_get_device(struct lis_api *impl, const char *dev_id,
		struct lis_item **item);


/* item functions */
static enum lis_error lis_sane_item_get_children(struct lis_item *self,
		struct lis_item ***children);
static enum lis_error lis_sane_item_get_options(struct lis_item *self,
		struct lis_option_descriptor ***descs);
static void lis_sane_item_close(struct lis_item *dev);


/* option descriptor functions */
static enum lis_error lis_sane_opt_get_value(struct lis_option_descriptor *opt,
		union lis_value *value);
static enum lis_error lis_sane_opt_set_value(struct lis_option_descriptor *self,
		const union lis_value value,
		int *set_flags);


static struct lis_api g_sane_impl_template = {
	.base_name = "sane",
	.cleanup = lis_sane_cleanup,
	.list_devices = lis_sane_list_devices,
	.get_device = lis_sane_get_device,
};


static struct lis_item g_sane_item_template = {
	.type = LIS_ITEM_UNIDENTIFIED,
	.get_children = lis_sane_item_get_children,
	.get_options = lis_sane_item_get_options,
	.get_scan_parameters = NULL, /* TODO */
	.scan_start = NULL, /* TODO */
	.close = lis_sane_item_close,
};


static struct lis_option_descriptor g_sane_option_template = {
	.fn = {
		.get_value = lis_sane_opt_get_value,
		.set_value = lis_sane_opt_set_value,
	},
};


/* sane implementation: root has no children */
static struct lis_item *g_children[] = { NULL };

/* ref counter */
static int g_sane_initialized = 0;


static void auth_callback(
		SANE_String_Const resource,
		SANE_Char username[SANE_MAX_USERNAME_LEN],
		SANE_Char password[SANE_MAX_PASSWORD_LEN]
	)
{
	/* TODO(Jflesch) */
	lis_log_warning("Authorization required to access scanner. Not supported yet");
	username[0] = '\0';
	password[0] = '\0';
}


static enum lis_error sane_status_to_lis_error(SANE_Status status)
{
	switch(status) {
		case SANE_STATUS_GOOD:
			return LIS_OK;
		case SANE_STATUS_UNSUPPORTED:
			return LIS_ERR_UNSUPPORTED;
		case SANE_STATUS_CANCELLED:
			return LIS_CANCELLED;
		case SANE_STATUS_DEVICE_BUSY:
			return LIS_ERR_DEVICE_BUSY;
		case SANE_STATUS_INVAL:
			return LIS_ERR_INVALID_VALUE;
		case SANE_STATUS_EOF:
			return LIS_END_OF_PAGE;
		case SANE_STATUS_JAMMED:
			return LIS_ERR_JAMMED;
		case SANE_STATUS_NO_DOCS:
			return LIS_END_OF_FEED;
		case SANE_STATUS_COVER_OPEN:
			return LIS_ERR_COVER_OPEN;
		case SANE_STATUS_IO_ERROR:
			return LIS_ERR_IO_ERROR;
		case SANE_STATUS_NO_MEM:
			return LIS_ERR_NO_MEM;
		case SANE_STATUS_ACCESS_DENIED:
			return LIS_ERR_ACCESS_DENIED;
	}
	lis_log_error("Unknown error code: %d", status);
	return LIS_ERR_INTERNAL_UNKNOWN_ERROR;
}


enum lis_error lis_api_sane(struct lis_api **out_impl)
{
	enum lis_error err;
	SANE_Int version_code = 0;
	struct lis_sane *private;

	private = calloc(1, sizeof(struct lis_sane));
	if (private == NULL) {
		lis_log_error("Out of memory");
		return LIS_ERR_NO_MEM;
	}

	if (g_sane_initialized <= 0) {
		lis_log_debug("sane_init() ...");
		err = sane_status_to_lis_error(sane_init(&version_code, auth_callback));
		lis_log_debug("sane_init(): 0x%X, %s", err, lis_strerror(err));
		if (LIS_IS_ERROR(err)) {
			free(private);
			return err;
		}
		lis_log_info("Sane version code: 0x%X", version_code);
		g_sane_initialized++;
	}

	memcpy(&private->parent, &g_sane_impl_template, sizeof(private->parent));
	*out_impl = &private->parent;
	return LIS_OK;
}


static void lis_sane_cleanup_dev_descriptors(struct lis_device_descriptor **dev_descs)
{
	int i;
	if (dev_descs == NULL) {
		return;
	}
	for (i = 0 ; dev_descs[i] != NULL ; i++) {
		free(dev_descs[i]->dev_id);
		free(dev_descs[i]->vendor);
		free(dev_descs[i]->model);
		free(dev_descs[i]->type);
		free(dev_descs[i]);
	}
	free(dev_descs);
}


static void lis_sane_cleanup(struct lis_api *impl)
{
	struct lis_sane *private = LIS_SANE_PRIVATE(impl);
	if (g_sane_initialized > 0) {
		lis_log_debug("sane_exit()");
		sane_exit();
		g_sane_initialized--;
		lis_log_debug("freeing Libinsane data ...");
		lis_sane_cleanup_dev_descriptors(private->dev_descs);
		free(private);
		lis_log_debug("Libinsane data freed ...");
	}
}


static enum lis_error lis_sane_list_devices(
		struct lis_api *impl, enum lis_device_locations locations,
		struct lis_device_descriptor ***out_dev_descs
	)
{
	struct lis_sane *private = LIS_SANE_PRIVATE(impl);
	enum lis_error err;
	const SANE_Device **dev_list = NULL;
	int nb_devs, i;
	int local_only = 0;

	switch(locations) {
		case LIS_DEVICE_LOCATIONS_ANY:
			local_only = 0;
			break;
		case LIS_DEVICE_LOCATIONS_LOCAL_ONLY:
			local_only = 1;
			break;
	}

	lis_log_debug("sane_get_devices() ...");
	err = sane_status_to_lis_error(sane_get_devices(&dev_list, local_only));
	lis_log_debug("sane_get_devices(): 0x%X, %s", err, lis_strerror(err));
	if (LIS_IS_ERROR(err)) {
		return err;
	}

	for (nb_devs = 0 ; dev_list[nb_devs] != NULL ; nb_devs++) {
	}

	*out_dev_descs = calloc(nb_devs + 1, sizeof(struct lis_device_descriptor *));
	if (*out_dev_descs == NULL) {
		lis_log_debug("out of memory");
		err = LIS_ERR_NO_MEM;
		goto error;
	}

	for (i = 0 ; dev_list[i] != NULL; i++) {
		(*out_dev_descs)[i] = calloc(1, sizeof(struct lis_device_descriptor));
		if ((*out_dev_descs)[i] == NULL) {
			lis_log_debug("out of memory");
			err = LIS_ERR_NO_MEM;
			goto error;
		}
		(*out_dev_descs)[i]->impl = impl;
		(*out_dev_descs)[i]->dev_id = strdup(dev_list[i]->name);
		(*out_dev_descs)[i]->vendor = strdup(dev_list[i]->vendor);
		(*out_dev_descs)[i]->model = strdup(dev_list[i]->model);
		(*out_dev_descs)[i]->type = strdup(dev_list[i]->type);
		if ((*out_dev_descs)[i]->dev_id == NULL
				|| (*out_dev_descs)[i]->vendor == NULL
				|| (*out_dev_descs)[i]->model == NULL
				|| (*out_dev_descs)[i]->type == NULL) {
			lis_log_debug("out of memory");
			err = LIS_ERR_NO_MEM;
			goto error;
		}
	}

	/* we must keep track of them so we can free them when list_devices() or cleanup()
	 * are called */
	lis_sane_cleanup_dev_descriptors(private->dev_descs);
	private->dev_descs = *out_dev_descs;
	return LIS_OK;

error:
	lis_sane_cleanup_dev_descriptors(*out_dev_descs);
	*out_dev_descs = NULL;
	return err;
}


static enum lis_error lis_sane_get_device(struct lis_api *impl, const char *dev_id, struct lis_item **item)
{
	struct lis_sane_item *private;
	enum lis_error err;

	private = calloc(1, sizeof(struct lis_sane_item));
	if (private == NULL) {
		lis_log_debug("out of memory");
		return LIS_ERR_NO_MEM;
	}
	memcpy(&private->parent, &g_sane_item_template, sizeof(private->parent));
	private->parent.name = strdup(dev_id);

	lis_log_debug("sane_open() ...")
	err = sane_status_to_lis_error(sane_open(dev_id, &private->handle));
	lis_log_debug("sane_open(): 0x%X, %s", err, lis_strerror(err));
	if (LIS_IS_ERROR(err)) {
		free(private);
		return err;
	}

	*item = &private->parent;
	return LIS_OK;
}

static void cleanup_options(struct lis_sane_item *private)
{
	int i;

	// all the pointers to strings have been stolen to libsane, so we don't have to free()
	// them

	for (i = 0 ; i < private->nb_opts ; i++) {
		if (private->options[i].parent.constraint.type == LIS_CONSTRAINT_LIST) {
			free(private->options[i].parent.constraint.possible.list.values);
			private->options[i].parent.constraint.possible.list.values = NULL;
		}
		free(private->options[i].value_buf);
		private->options[i].value_buf = NULL;
	}

	free(private->options);
	private->options = NULL;
	free(private->option_ptrs);
	private->option_ptrs = NULL;
}

static void lis_sane_item_close(struct lis_item *device)
{
	struct lis_sane_item *private = LIS_SANE_ITEM_PRIVATE(device);
	cleanup_options(private);
	lis_log_debug("sane_close()");
	free((void *)private->parent.name);
	sane_close(private->handle);
}


static enum lis_error lis_sane_item_get_children(struct lis_item *self, struct lis_item ***children)
{
	LIS_UNUSED(self);
	*children = g_children;
	return LIS_OK;
}


static enum lis_value_type sane_type_to_lis_type(SANE_Value_Type type)
{
	switch(type)
	{
		case SANE_TYPE_BOOL:
			return LIS_TYPE_BOOL;
		case SANE_TYPE_INT:
			return LIS_TYPE_INTEGER;
		case SANE_TYPE_FIXED:
			return LIS_TYPE_DOUBLE;
		case SANE_TYPE_STRING:
			return LIS_TYPE_STRING;
		case SANE_TYPE_BUTTON:
			assert(type != SANE_TYPE_BUTTON);
			lis_log_warning("Unsupported option value type: SANE_TYPE_BUTTON");
			return LIS_ENUM_ERROR;
		case SANE_TYPE_GROUP:
			assert(type != SANE_TYPE_GROUP);
			lis_log_warning("Unsupported option value type: SANE_TYPE_GROUP");
			return LIS_ENUM_ERROR;
	}
	assert(0);
	lis_log_warning("Unsupported option value type: %d", type);
	return LIS_ENUM_ERROR;
}


static union lis_value sane_word_to_lis_value(enum lis_value_type type, SANE_Word word)
{
	union lis_value val;
	memset(&val, 0, sizeof(val));
	switch(type)
	{
		case LIS_TYPE_BOOL:
			val.boolean = (word > 0);
			return val;
		case LIS_TYPE_INTEGER:
			val.integer = word;
			return val;
		case LIS_TYPE_DOUBLE:
			val.dbl = SANE_UNFIX(word);
			return val;
		case LIS_TYPE_STRING:
		case LIS_TYPE_IMAGE_FORMAT:
			lis_log_error("Got unexpected type: %d", type);
			assert(type != LIS_TYPE_STRING);
			assert(type != LIS_TYPE_IMAGE_FORMAT);
			return val;
	}
	lis_log_error("Got unexpected type: %d", type);
	assert(0);
	return val;
}


static int sane_cap_to_lis_capabilities(SANE_Int sane_cap)
{
	int lis_cap = 0;

	if (sane_cap & SANE_CAP_EMULATED) {
		lis_cap |= LIS_CAP_EMULATED;
	}
	if (sane_cap & SANE_CAP_AUTOMATIC) {
		lis_cap |= LIS_CAP_AUTOMATIC;
	}
	if (sane_cap & SANE_CAP_HARD_SELECT) {
		lis_cap |= LIS_CAP_HW_SELECT;
	}
	if (sane_cap & SANE_CAP_SOFT_SELECT) {
		lis_cap |= LIS_CAP_SW_SELECT;
	}
	if (sane_cap & SANE_CAP_INACTIVE) {
		lis_cap |= LIS_CAP_INACTIVE;
	}

	return lis_cap;
}

static enum lis_unit sane_unit_to_lis_unit(SANE_Unit sane_unit)
{
	switch(sane_unit) {
		case SANE_UNIT_NONE:
			return LIS_UNIT_NONE;
		case SANE_UNIT_PIXEL:
			return LIS_UNIT_PIXEL;
		case SANE_UNIT_BIT:
			return LIS_UNIT_BIT;
		case SANE_UNIT_MM:
			return LIS_UNIT_MM;
		case SANE_UNIT_DPI:
			return LIS_UNIT_DPI;
		case SANE_UNIT_PERCENT:
			return LIS_UNIT_PERCENT;
		case SANE_UNIT_MICROSECOND:
			return LIS_UNIT_MICROSECOND;
	}
	lis_log_warning("Unknown unit: %d", sane_unit);
	return LIS_UNIT_NONE;
}


static struct lis_value_range sane_range_to_lis_range(enum lis_value_type type, const SANE_Range *sane_range)
{
	struct lis_value_range lis_range = {{ 0 }};

	assert(type == LIS_TYPE_INTEGER || type == LIS_TYPE_DOUBLE);

	lis_range.min = sane_word_to_lis_value(type, sane_range->min);
	lis_range.max = sane_word_to_lis_value(type, sane_range->max);
	lis_range.interval = sane_word_to_lis_value(type, sane_range->quant);
	return lis_range;
}


static struct lis_value_list sane_word_list_to_lis_list(enum lis_value_type type, const SANE_Word *sane_list)
{
	struct lis_value_list lis_list = { 0 };
	int i;

	lis_list.values = calloc(sane_list[0], sizeof(union lis_value));
	if (lis_list.values == NULL) {
		lis_log_error("Out of memory");
		return lis_list;
	}
	lis_list.nb_values = sane_list[0] - 1;

	for (i = 1 ; i < sane_list[0] ; i++) {
		switch(type)
		{
			case LIS_TYPE_BOOL:
				lis_list.values[i - 1].boolean = sane_list[i];
				break;
			case LIS_TYPE_INTEGER:
				lis_list.values[i - 1].integer = sane_list[i];
				break;
			case LIS_TYPE_DOUBLE:
				lis_list.values[i - 1].dbl = SANE_UNFIX(sane_list[i]);
				break;
			case LIS_TYPE_STRING:
				assert(type != LIS_TYPE_STRING);
				break;
			case LIS_TYPE_IMAGE_FORMAT:
				assert(type != LIS_TYPE_IMAGE_FORMAT);
				break;
		}
	}
	return lis_list;
}

static struct lis_value_list sane_string_list_to_lis_list(enum lis_value_type type,
		const SANE_String_Const *sane_list)
{
	struct lis_value_list lis_list = { 0 };
	int nb_values;
	int i;

	for (nb_values = 0 ; sane_list[nb_values] != NULL ; nb_values++) {
	}

	lis_list.values = calloc(nb_values + 1, sizeof(union lis_value));
	if (lis_list.values == NULL) {
		lis_log_error("Out of memory");
		return lis_list;
	}
	lis_list.nb_values = nb_values;

	for (i = 0 ; i < nb_values ; i++) {
		// pointers provided by sane are valid until sane_close()
		// same as ours --> no need to duplicate strings
		lis_list.values[i].string = sane_list[i];
	}

	return lis_list;
}


static int lis_sane_check_opt_descriptor(const char *item_name, int opt_idx,
		const SANE_Option_Descriptor *sane_desc)
{
	/* Integrity check of the option descriptor, becauxse there are some
	 * weird things in this world ... */

	int has_expected_size = 1;
	int expected_size = -1;

	if (sane_desc->name == NULL || sane_desc->name[0] == '\0') {
		// Seen on: Sane test backend + Brother DS-620 driver
		// crappy option descriptor from the test backend of Sane
		lis_log_warning("Invalid option descriptor: missing option name");
		goto failed;
	}

	switch(sane_desc->type)
	{
		case SANE_TYPE_BOOL:
		case SANE_TYPE_INT:
		case SANE_TYPE_FIXED:
			expected_size = sizeof(int);
			break;
		case SANE_TYPE_STRING:
			has_expected_size = 0;
			break;
		case SANE_TYPE_BUTTON:
		case SANE_TYPE_GROUP:
			lis_log_warning("Unsupported option type: %s=%d",
					sane_desc->name, sane_desc->type);
			goto failed;
	}

	if (has_expected_size) {
		if (expected_size != sane_desc->size) {
			lis_log_warning("Unexpected value size for option %s(%d) = %dB. Expected: %dB",
					sane_desc->name, sane_desc->type, sane_desc->size,
					expected_size);
			goto failed;
		}
	}

	return 1;

failed:
	lis_log_warning(
		"Device [%s]: Invalid or unsupported option descriptor [%s](%d ; [%s] ; [%s])."
		" Ignored",
		item_name,
		sane_desc->name != NULL ? sane_desc->name : "(null)",
		opt_idx,
		sane_desc->title != NULL ? sane_desc->title : "(null)",
		sane_desc->desc != NULL ? sane_desc->desc : "(null)"
	);
	return 0;
}


static enum lis_error lis_sane_item_get_options(struct lis_item *self,
		struct lis_option_descriptor ***descs)
{
	struct lis_sane_item *private = LIS_SANE_ITEM_PRIVATE(self);
	enum lis_error err;
	int nb_opts, in, out;
	const SANE_Option_Descriptor *sane_desc;

	err = sane_status_to_lis_error(sane_control_option(
		private->handle, 0 /* option 0 = number of options */,
		SANE_ACTION_GET_VALUE, &nb_opts, NULL
	));
	assert(LIS_IS_OK(err));
	if (LIS_IS_ERROR(err)) {
		lis_log_error("%s->sane_control_option(NUMBER OF OPTIONS): %d, %s",
				self->name, err, lis_strerror(err));
		return err;
	}
	lis_log_debug("%s->sane_control_option(NUMBER OF OPTIONS): %d", self->name, nb_opts);
	assert(nb_opts > 1);
	if (nb_opts <= 1) {
		lis_log_error("Unexpected number of options on the device %s: %d",
				self->name, nb_opts);
	}
	nb_opts--; // number of options include the first option, which is the number of options

	cleanup_options(private);
	private->options = calloc(nb_opts, sizeof(struct lis_sane_option));
	private->option_ptrs = calloc(nb_opts + 1, sizeof(struct lis_option_descriptor *));
	if (private->options == NULL || private->option_ptrs == NULL) {
		lis_log_error("sane get options: Out of memory");
		err = LIS_ERR_NO_MEM;
		goto error;
	}
	for (out = 0 ; out < nb_opts ; out++) {
		memcpy(&private->options[out].parent, &g_sane_option_template,
				sizeof(private->options[out].parent));
	}

	for (in = 0, out = 0 ; in < nb_opts ; in++) {
		lis_log_debug("%s->sane_get_option_descriptor(%d) ...", self->name, in + 1);
		sane_desc = sane_get_option_descriptor(private->handle, in + 1);
		if (sane_desc == NULL) {
			// shouldn't happen --> if it does: Sane bug ?
			lis_log_error("Unknown error while getting info on option %s[%d]",
				self->name, in + 1);
			err = LIS_ERR_INTERNAL_UNKNOWN_ERROR;
			goto error;
		}

		if (!lis_sane_check_opt_descriptor(self->name, in + 1, sane_desc)) {
			continue;
		}

		lis_log_debug("Option: %s->%s(%d) (%s, %s)",
			self->name, sane_desc->name, in + 1, sane_desc->title, sane_desc->desc)

		if (sane_desc->size <= 0) {
			lis_log_warning("Unsupported option value size: %s=%d. Option skipped",
				sane_desc->name, sane_desc->size);
			continue;
		}

		private->options[out].item = private;
		private->options[out].opt_idx = in + 1;
		private->options[out].value_size = sane_desc->size;
		// reuse libsane pointers: they are valid until we call sane_close()
		// --> same as us
		private->options[out].parent.name = sane_desc->name;
		private->options[out].parent.title = sane_desc->title;
		private->options[out].parent.desc = sane_desc->desc;
		private->options[out].parent.capabilities = sane_cap_to_lis_capabilities(sane_desc->cap);

		private->options[out].parent.value.type = sane_type_to_lis_type(sane_desc->type);
		private->options[out].parent.value.unit = sane_unit_to_lis_unit(sane_desc->unit);

		private->options[out].parent.constraint.type = LIS_CONSTRAINT_NONE;

		switch(sane_desc->constraint_type) {
			case SANE_CONSTRAINT_NONE:
				break;
			case SANE_CONSTRAINT_RANGE:
				private->options[out].parent.constraint.type = LIS_CONSTRAINT_RANGE;
				private->options[out].parent.constraint.possible.range =
					sane_range_to_lis_range(
						private->options[out].parent.value.type,
						sane_desc->constraint.range
					);
				break;
			case SANE_CONSTRAINT_WORD_LIST:
				assert(private->options[out].parent.value.type != LIS_TYPE_STRING);
				private->options[out].parent.constraint.type = LIS_CONSTRAINT_LIST;
				private->options[out].parent.constraint.possible.list =
					sane_word_list_to_lis_list(
						private->options[out].parent.value.type,
						sane_desc->constraint.word_list
					);
				break;
			case SANE_CONSTRAINT_STRING_LIST:
				assert(private->options[out].parent.value.type == LIS_TYPE_STRING);
				private->options[out].parent.constraint.type = LIS_CONSTRAINT_LIST;
				private->options[out].parent.constraint.possible.list =
					sane_string_list_to_lis_list(
						private->options[out].parent.value.type,
						sane_desc->constraint.string_list
					);
				break;
		}

		private->option_ptrs[out] = &private->options[out].parent;
		out++;
	}

	*descs = private->option_ptrs;
	return LIS_OK;

error:
	cleanup_options(private);
	return err;
}


static enum lis_error control_sane_value(
	struct lis_sane_option *private,
	SANE_Action action,
	union lis_value *value,
	int *set_flags)
{
	SANE_Status sane_status;
	int fixed_value;

	switch(private->parent.value.type) {
		case LIS_TYPE_BOOL:
			assert(private->value_size == sizeof(value->boolean));
			sane_status = sane_control_option(
				private->item->handle,
				private->opt_idx,
				action,
				&value->boolean,
				set_flags
			);
			return sane_status_to_lis_error(sane_status);
		case LIS_TYPE_INTEGER:
			assert(private->value_size == sizeof(value->integer));
			sane_status = sane_control_option(
				private->item->handle,
				private->opt_idx,
				action,
				&value->integer,
				set_flags
			);
			return sane_status_to_lis_error(sane_status);
		case LIS_TYPE_DOUBLE:
			/* Sane stores doubles as integer by "fixing" them */
			assert(private->value_size == sizeof(int));
			if (action == SANE_ACTION_SET_VALUE) {
				fixed_value = SANE_FIX(value->dbl);
			}
			sane_status = sane_control_option(
				private->item->handle,
				private->opt_idx,
				action,
				&fixed_value,
				set_flags
			);
			if (action == SANE_ACTION_GET_VALUE) {
				value->dbl = SANE_UNFIX(fixed_value);
			}
			return sane_status_to_lis_error(sane_status);
		case LIS_TYPE_STRING:
			switch(action) {
				case SANE_ACTION_GET_VALUE:
					free(private->value_buf);
					private->value_buf = malloc(private->value_size);
					if (private->value_buf == NULL) {
						lis_log_error("Out of memory");
						return LIS_ERR_NO_MEM;
					}
					sane_status = sane_control_option(
						private->item->handle,
						private->opt_idx,
						SANE_ACTION_GET_VALUE,
						private->value_buf,
						set_flags
					);
					if (sane_status == SANE_STATUS_GOOD) {
						value->string = private->value_buf;
					} else {
						value->string = NULL;
					}
					break;
				case SANE_ACTION_SET_VALUE:
					sane_status = sane_control_option(
						private->item->handle,
						private->opt_idx,
						SANE_ACTION_SET_VALUE,
						(void *)value->string, // strip const attribute
						set_flags
					);
					break;
				case SANE_ACTION_SET_AUTO:
					assert(action != SANE_ACTION_SET_AUTO);
					lis_log_error("Unsupported action: SANE_ACTION_SET_AUTO");
					return LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
			}
			return sane_status_to_lis_error(sane_status);
		case LIS_TYPE_IMAGE_FORMAT:
			assert(private->parent.value.type == LIS_TYPE_IMAGE_FORMAT);
			break;
	}

	lis_log_error("Unknown value type: %d", private->parent.value.type);
	assert(0);
	return LIS_ERR_INTERNAL_UNKNOWN_ERROR;
}


static enum lis_error lis_sane_opt_get_value(struct lis_option_descriptor *self,
		union lis_value *value)
{
	struct lis_sane_option *private = LIS_SANE_OPTION(self);
	enum lis_error err;

	lis_log_debug("%s->%s->sane_control_option(GET_VALUE) ...",
			private->item->parent.name, self->name);
	err = control_sane_value(
		private,
		SANE_ACTION_GET_VALUE,
		value,
		NULL
	);
	lis_log_debug("%s->%s->sane_control_option(GET_VALUE): %d, %s",
			private->item->parent.name, self->name,
			err, lis_strerror(err));
	if (LIS_IS_ERROR(err)) {
		lis_log_error("%s->%s->sane_control_option(GET_VALUE) failed: %d, %s",
			private->item->parent.name, self->name,
			err, lis_strerror(err))
		return err;
	}

	return LIS_OK;
}


static enum lis_error lis_sane_opt_set_value(struct lis_option_descriptor *self,
		union lis_value value,
		int *set_flags)
{
	struct lis_sane_option *private = LIS_SANE_OPTION(self);
	enum lis_error err;
	int sane_set_flags = 0;

	lis_log_debug("%s->%s->sane_control_option(SET_VALUE) ...",
			private->item->parent.name, self->name);
	err = control_sane_value(
		private,
		SANE_ACTION_SET_VALUE,
		&value,
		&sane_set_flags
	);
	lis_log_debug("%s->%s->sane_control_option(SET_VALUE): %d, %s",
			private->item->parent.name, self->name,
			err, lis_strerror(err));
	if (LIS_IS_ERROR(err)) {
		lis_log_error("%s->%s->sane_control_option(SET_VALUE) failed: %d, %s",
			private->item->parent.name, self->name,
			err, lis_strerror(err))
		return err;
	}

	if (set_flags != NULL) {
		*set_flags = 0;
		if (sane_set_flags & SANE_INFO_INEXACT) {
			*set_flags |= LIS_SET_FLAG_INEXACT;
		}
		if (sane_set_flags & SANE_INFO_RELOAD_OPTIONS) {
			*set_flags |= LIS_SET_FLAG_MUST_RELOAD_OPTIONS;
		}
		if (sane_set_flags & SANE_INFO_RELOAD_PARAMS) {
			*set_flags |= LIS_SET_FLAG_MUST_RELOAD_PARAMS;
		}
	}

	return LIS_OK;
}
