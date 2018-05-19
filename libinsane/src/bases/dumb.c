#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libinsane/capi.h>
#include <libinsane/dumb.h>
#include <libinsane/error.h>
#include <libinsane/util.h>


static void dumb_cleanup(struct lis_api *impl);
static enum lis_error dumb_list_devices(
	struct lis_api *impl, int local_only, struct lis_device_descriptor ***dev_infos
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

	struct lis_device_descriptor **descs;
	struct lis_dumb_item **devices;
};
#define LIS_DUMB_PRIVATE(impl) ((struct lis_dumb_private *)(impl))

static struct lis_item g_dumb_item_template = {
	0
};

static struct lis_api g_dumb_api_template = {
	.base_name = NULL,
	.cleanup = dumb_cleanup,
	.list_devices = dumb_list_devices,
	.get_device = dumb_get_device,
};


static void dumb_cleanup(struct lis_api *self)
{
	free((void*)self->base_name);
}


static enum lis_error dumb_list_devices(
		struct lis_api *self, int local_only, struct lis_device_descriptor ***dev_infos
	)
{
	struct lis_dumb_private *private = LIS_DUMB_PRIVATE(self);
	*dev_infos = private->descs;
	return LIS_OK;
}

static enum lis_error dumb_get_device(
			struct lis_api *self, const char *dev_id, struct lis_item **item
		) {
	struct lis_dumb_private *private = LIS_DUMB_PRIVATE(self);
	int i;

	for (i = 0 ; private->devices[i] != NULL ; i++) {
		if (strcmp(dev_id, private->devices[i]->dev_id) == 0) {
			*item = &private->devices[i]->base;
			return LIS_OK;
		}
	}

	return LIS_ERR_INVALID_VALUE;
}


enum lis_error lis_api_dumb(struct lis_api **impl) {
	return lis_api_dumb_named_devices(impl, "dumb", 0);
}


enum lis_error lis_api_dumb_named_devices(
		struct lis_api **out_impl, const char *name, int nb_devices
) {
	int i;
	struct lis_dumb_private *private;
	struct lis_dumb_item *item;

	private = calloc(1, sizeof(struct lis_dumb_private));
	memcpy(&private->base, &g_dumb_api_template, sizeof(private->base));
	private->base.base_name = strdup(name);

	private->descs = calloc(nb_devices + 1, sizeof(struct lis_device_descriptor *));
	for (i = 0 ; i < nb_devices ; i++) {
		private->descs[i] = calloc(1, sizeof(struct lis_device_descriptor));
		private->descs[i]->impl = &private->base;
		asprintf(&private->descs[i]->dev_id, "dumb_:#$@;dev%d", i);
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

	*out_impl = &private->base;
	return LIS_OK;
}
