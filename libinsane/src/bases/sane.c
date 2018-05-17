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

struct lis_sane_item
{
	struct lis_item item;
	SANE_Handle handle;
};
#define LIS_SANE_ITEM_PRIVATE(impl) ((struct lis_sane_item *)(impl))


static void lis_sane_cleanup(struct lis_api *impl);
static enum lis_error lis_sane_list_devices(struct lis_api *impl, int local_only,
		struct lis_device_descriptor ***out_dev_descs);
static enum lis_error lis_sane_get_device(struct lis_api *impl, const char *dev_id,
		struct lis_item **item);

static struct lis_api g_sane_impl_template = {
	.base_name = "sane",
	.cleanup = lis_sane_cleanup,
	.list_devices = lis_sane_list_devices,
	.get_device = lis_sane_get_device,
};


static void lis_sane_item_close(struct lis_item *dev);

static struct lis_item g_sane_item_template = {
	.type = LIS_ITEM_UNIDENTIFIED,
	.get_children = NULL, /* TODO */
	.get_options = NULL, /* TODO */
	.get_scan_parameters = NULL, /* TODO */
	.scan_start = NULL, /* TODO */
	.close = lis_sane_item_close,
};


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
		struct lis_api *impl, int local_only, struct lis_device_descriptor ***out_dev_descs
	)
{
	struct lis_sane *private = LIS_SANE_PRIVATE(impl);
	enum lis_error err;
	const SANE_Device **dev_list = NULL;
	int nb_devs, i;

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
	memcpy(&private->item, &g_sane_item_template, sizeof(private->item));
	private->item.name = strdup(dev_id);

	lis_log_debug("sane_open() ...")
	err = sane_status_to_lis_error(sane_open(dev_id, &private->handle));
	lis_log_debug("sane_open(): 0x%X, %s", err, lis_strerror(err));
	if (LIS_IS_ERROR(err)) {
		free(private);
		return err;
	}

	*item = &private->item;
	return LIS_OK;
}


static void lis_sane_item_close(struct lis_item *device)
{
	struct lis_sane_item *private = LIS_SANE_ITEM_PRIVATE(device);
	lis_log_debug("sane_close()");
	free((void *)private->item.name);
	sane_close(private->handle);
}

