#include <string.h>

#include <libinsane/capi.h>
#include <libinsane/error.h>
#include <libinsane/log.h>
#include <libinsane/multiplexer.h>


struct lis_multi
{
	struct lis_api parent;

	struct lis_api **impls;
	int nb_impls;
};
#define LIS_MULTI_PRIVATE(impl) ((struct lis_multi *)(impl))


static void lis_multi_cleanup(struct lis_api *impl);
static enum lis_error lis_multi_get_devices(struct lis_api *impl, struct lis_device_descriptor ***dev_infos);
static enum lis_error lis_multi_dev_get(struct lis_api *impl, const char *dev_id, struct lis_item **item);


static struct lis_api g_multi_impl_template = {
	.cleanup = lis_multi_cleanup,
	.get_devices = lis_multi_get_devices,
	.dev_get = lis_multi_dev_get,
};


enum lis_error lis_api_multiplexer(
		struct lis_api **input_implementations, int nb_input_implementations,
		struct lis_api **output_implementation
	)
{
	struct lis_multi *multi;

	multi = calloc(1, sizeof(struct lis_multi));
	if (multi == NULL) {
		lis_log_error("Out of memory");
		return LIS_ERR_NO_MEM;
	}
	multi->impls = calloc(nb_input_implementations, sizeof(struct lis_api *));
	if (multi->impls == NULL) {
		free(multi);
		lis_log_error("Out of memory");
		return LIS_ERR_NO_MEM;
	}

	memcpy(&multi->parent, &g_multi_impl_template, sizeof(multi->parent));
	memcpy(&multi->impls, input_implementations, nb_input_implementations * sizeof(struct lis_api*));
	multi->nb_impls = nb_input_implementations;
	*output_implementation = &multi->parent;
	return LIS_OK;
}

static void lis_multi_cleanup(struct lis_api *impl)
{
	struct lis_multi *multi = LIS_MULTI_PRIVATE(impl);
	int i;

	for (i = 0 ; i < multi->nb_impls ; i++) {
		multi->impls[i]->cleanup(multi->impls[i]);
	}
	free(multi);
}

static enum lis_error lis_multi_get_devices(struct lis_api *impl, struct lis_device_descriptor ***dev_infos)
{
	/* TODO */
	return LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
}

static enum lis_error lis_multi_dev_get(struct lis_api *impl, const char *dev_id, struct lis_item **item)
{
	/* TODO */
	return LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
}
