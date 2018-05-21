#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libinsane/capi.h>
#include <libinsane/dumb.h>
#include <libinsane/error.h>
#include <libinsane/log.h>
#include <libinsane/util.h>


static void dumb_cleanup(struct lis_api *impl);
static enum lis_error dumb_list_devices(
	struct lis_api *impl, enum lis_device_locations, struct lis_device_descriptor ***dev_infos
);
static enum lis_error dumb_get_device(
	struct lis_api *impl, const char *dev_id, struct lis_item **item
);

struct lis_dumb_item {
	struct lis_item base;
	const char *dev_id;
};
#define LIS_DUMB_ITEM(impl) ((struct lis_dumb_item *)(impl))

struct lis_dumb_private {
	struct lis_api base;

	enum lis_error list_devices_ret;
	struct lis_device_descriptor **descs;

	enum lis_error get_device_ret;
	struct lis_dumb_item **devices;
};
#define LIS_DUMB_PRIVATE(impl) ((struct lis_dumb_private *)(impl))

static struct lis_item g_dumb_item_template = {
	.name = "dumb-o-jet",
};

static struct lis_api g_dumb_api_template = {
	.base_name = NULL,
	.cleanup = dumb_cleanup,
	.list_devices = dumb_list_devices,
	.get_device = dumb_get_device,
};

static struct lis_device_descriptor *g_dumb_default_devices[] = { NULL };

static void dumb_cleanup_descs(struct lis_device_descriptor **descs)
{
	int i;

	if (descs == g_dumb_default_devices) {
		return;
	}

	for (i = 0 ; descs[i] != NULL ; i++) {
		free(descs[i]->dev_id);
		free(descs[i]);
	}

	free(descs);
}

static void dumb_cleanup_devices(struct lis_dumb_item **devs)
{
	int i;
	if (devs == NULL) {
		return;
	}

	for (i = 0 ; devs[i] != NULL ; i++) {
		free(devs[i]);
	}

	free(devs);
}

static void dumb_cleanup(struct lis_api *self)
{
	struct lis_dumb_private *private = LIS_DUMB_PRIVATE(self);
	free((void*)self->base_name);

	dumb_cleanup_descs(private->descs);
	dumb_cleanup_devices(private->devices);
	free(private);
}


static enum lis_error dumb_list_devices(
		struct lis_api *self, enum lis_device_locations locations,
		struct lis_device_descriptor ***dev_infos
	)
{
	LIS_UNUSED(locations);

	struct lis_dumb_private *private = LIS_DUMB_PRIVATE(self);
	if (LIS_IS_OK(private->list_devices_ret)) {
		*dev_infos = private->descs;
	} else {
		*dev_infos = NULL;
	}
	return private->list_devices_ret;
}

static enum lis_error dumb_get_device(
		struct lis_api *self, const char *dev_id, struct lis_item **item
	)
{
	struct lis_dumb_private *private = LIS_DUMB_PRIVATE(self);
	int i;

	if (private->devices == NULL) {
		lis_log_error("[dumb] get_device() called when no device has been set"
				"; shouldn't happen");
		return LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
	}

	if (LIS_IS_OK(private->get_device_ret)) {
		for (i = 0 ; private->devices[i] != NULL ; i++) {
			if (strcmp(dev_id, private->devices[i]->dev_id) == 0) {
				*item = &private->devices[i]->base;
				return LIS_OK;
			}
		}
		return LIS_ERR_INVALID_VALUE;
	} else {
		return private->get_device_ret;
	}
}


enum lis_error lis_api_dumb(struct lis_api **out_impl, const char *name)
{
	struct lis_dumb_private *private;

	private = calloc(1, sizeof(struct lis_dumb_private));
	memcpy(&private->base, &g_dumb_api_template, sizeof(private->base));
	private->base.base_name = strdup(name);

	private->list_devices_ret = LIS_OK;
	private->descs = g_dumb_default_devices;
	private->get_device_ret = LIS_OK;

	*out_impl = &private->base;
	return LIS_OK;
}


void lis_dumb_set_nb_devices(struct lis_api *self, int nb_devices)
{
	struct lis_dumb_private *private = LIS_DUMB_PRIVATE(self);
	int i;
	struct lis_dumb_item *item;

	private->descs = calloc(nb_devices + 1, sizeof(struct lis_device_descriptor *));
	for (i = 0 ; i < nb_devices ; i++) {
		private->descs[i] = calloc(1, sizeof(struct lis_device_descriptor));
		private->descs[i]->impl = &private->base;
		asprintf(&private->descs[i]->dev_id, "dumb dev%d", i);
		private->descs[i]->vendor = "Microsoft";
		private->descs[i]->model = "Bugware";
		private->descs[i]->type = NULL;
	}

	private->devices = calloc(nb_devices + 1, sizeof(struct lis_dumb_item *));
	for (i = 0 ; i < nb_devices ; i++) {
		item = calloc(1, sizeof(struct lis_dumb_item));
		memcpy(&item->base, &g_dumb_item_template, sizeof(item->base));
		item->dev_id = private->descs[i]->dev_id;
		private->devices[i] = item;
	}
}

void lis_dumb_set_list_devices_return(struct lis_api *self, enum lis_error ret)
{
	struct lis_dumb_private *private = LIS_DUMB_PRIVATE(self);
	private->list_devices_ret = ret;
}

void lis_dumb_set_get_device_return(struct lis_api *self, enum lis_error ret)
{
	struct lis_dumb_private *private = LIS_DUMB_PRIVATE(self);
	private->get_device_ret = ret;
}
