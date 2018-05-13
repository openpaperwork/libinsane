#include <libinsane/capi.h>
#include <libinsane/dumb.h>
#include <libinsane/error.h>
#include <libinsane/util.h>


static void dumb_cleanup(struct lis_api *impl);
static enum lis_error dumb_get_devices(
	struct lis_api *impl, int local_only, struct lis_device_descriptor ***dev_infos
);
static enum lis_error dumb_dev_get(
	struct lis_api *impl, const char *dev_id, struct lis_item **item
);


static struct lis_device_descriptor *g_dev_infos[] = {
	NULL
};


static struct lis_api g_dumb_api = {
	.cleanup = dumb_cleanup,
	.get_devices = dumb_get_devices,
	.dev_get = dumb_dev_get,
};


static void dumb_cleanup(struct lis_api *impl)
{
	LIS_UNUSED(impl);
}


static enum lis_error dumb_get_devices(
		struct lis_api *impl, int local_only, struct lis_device_descriptor ***dev_infos
	)
{
	LIS_UNUSED(impl);
	*dev_infos = g_dev_infos;
	return LIS_OK;
}

static enum lis_error dumb_dev_get(
	struct lis_api *impl, const char *dev_id, struct lis_item **item
) {
	LIS_UNUSED(impl);
	LIS_UNUSED(dev_id);
	LIS_UNUSED(item);
	return LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
}


enum lis_error lis_api_dumb(struct lis_api **api) {
	*api = &g_dumb_api;
	return LIS_OK;
}
