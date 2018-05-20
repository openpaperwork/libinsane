#define _GNU_SOURCE

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <libinsane/capi.h>
#include <libinsane/error.h>
#include <libinsane/log.h>
#include <libinsane/multiplexer.h>


#define MAX_APIS 8


struct lis_multi
{
	struct lis_api parent;

	struct lis_api **impls;
	int nb_impls;

	/* list actually reported to the caller.
	 * IMPORTANT: device ids are prefixed with "<api_name>:"
	 * So:
	 * - struct lis_device_descriptor are *not* the same than those reported by each child implementation
	 * - (struct lis_device_descriptor)->dev_id are *not* the same than those reported by each child implementation
	 * - all other pointers are those reported by the child implementation (--> no need to free them here)
	 */
	struct lis_device_descriptor **merged_devs;
};
#define LIS_MULTI_PRIVATE(impl) ((struct lis_multi *)(impl))


static void lis_multi_cleanup(struct lis_api *impl);
static enum lis_error lis_multi_list_devices(
	struct lis_api *impl, enum lis_device_locations locations,
	struct lis_device_descriptor ***dev_descs
);
static enum lis_error lis_multi_get_device(struct lis_api *impl, const char *dev_id, struct lis_item **item);


static struct lis_api g_multi_impl_template = {
	.cleanup = lis_multi_cleanup,
	.list_devices = lis_multi_list_devices,
	.get_device = lis_multi_get_device,
};


enum lis_error lis_api_multiplexer(
		struct lis_api **input_implementations, int nb_input_implementations,
		struct lis_api **output_implementation
	)
{
	struct lis_multi *private;

	if (nb_input_implementations > MAX_APIS || nb_input_implementations == 0) {
		lis_log_error("Too many implementations to manage ! (%d > %d)",
			nb_input_implementations, MAX_APIS);
		return LIS_ERR_INVALID_VALUE;
	}

	private = calloc(1, sizeof(struct lis_multi));
	if (private == NULL) {
		lis_log_error("Out of memory");
		return LIS_ERR_NO_MEM;
	}
	private->impls = calloc(nb_input_implementations, sizeof(struct lis_api *));
	if (private->impls == NULL) {
		free(private);
		lis_log_error("Out of memory");
		return LIS_ERR_NO_MEM;
	}

	memcpy(&private->parent, &g_multi_impl_template, sizeof(private->parent));
	memcpy(private->impls, input_implementations, nb_input_implementations * sizeof(struct lis_api*));
	private->nb_impls = nb_input_implementations;
	*output_implementation = &private->parent;
	return LIS_OK;
}


static void lis_multi_cleanup_dev_descs(struct lis_device_descriptor **dev_descs) {
	int i;
	if (dev_descs == NULL) {
		return;
	}
	for (i = 0 ; dev_descs[i] != NULL ; i++) {
		free(dev_descs[i]->dev_id);
		free(dev_descs[i]);
	}
	free(dev_descs[i]);
	free(dev_descs);
}


static void lis_multi_cleanup(struct lis_api *impl)
{
	struct lis_multi *private = LIS_MULTI_PRIVATE(impl);
	int i;

	lis_multi_cleanup_dev_descs(private->merged_devs);
	for (i = 0 ; i < private->nb_impls ; i++) {
		private->impls[i]->cleanup(private->impls[i]);
	}
	free(private->impls);
	free(private);
}


static enum lis_error lis_multi_list_devices(
		struct lis_api *impl, enum lis_device_locations locations,
		struct lis_device_descriptor ***out_dev_descs)
{
	struct lis_multi *private = LIS_MULTI_PRIVATE(impl);
	enum lis_error err, last_err = LIS_OK;
	int has_success = 0, i, j, nb_devs;
	struct lis_device_descriptor **devs[MAX_APIS];

	assert(private->nb_impls > 0);
	assert(private->nb_impls <= MAX_APIS);

	*out_dev_descs = NULL;

	/* get all the devices */
	memset(&devs, 0, sizeof(devs));
	nb_devs = 0;
	for (i = 0 ; i < private->nb_impls ; i++) {
		lis_log_debug("Getting devices from API %d", i);
		err = private->impls[i]->list_devices(private->impls[i], locations, &devs[i]);
		if (LIS_IS_ERROR(err)) {
			last_err = err;
			continue;
		}
		has_success = 1;
		for (j = 0 ; devs[i][j] != NULL ; j++) {
			nb_devs++;
		}
		lis_log_debug("Got %d devices from API %d", j, i);
	}

	/* if all implementations have failed
	 * or if at least one failed and no device has been found by any other */
	if (!has_success || (nb_devs == 0 && LIS_IS_ERROR(last_err))) {
		lis_log_debug("list_devices() has failed:"
			" had success ? %d ;"
			" number of devices found: %d ;"
			" last error: 0x%X, %s",
			has_success, nb_devs, last_err, lis_strerror(last_err));
		return last_err;
	}

	/* merge the device lists */
	*out_dev_descs = calloc(nb_devs + 1, sizeof(struct lis_device_descriptor *));
	if (*out_dev_descs == NULL) {
		lis_log_error("out of memory");
		err = LIS_ERR_NO_MEM;
		goto error;
	}
	nb_devs = 0;
	for (i = 0 ; i < private->nb_impls ; i++) {
		if (devs[i] == NULL) {
			continue;
		}
		for (j = 0 ; devs[i][j] != NULL ; j++) {
			(*out_dev_descs)[nb_devs] = calloc(1, sizeof(struct lis_device_descriptor));
			if ((*out_dev_descs)[nb_devs] == NULL) {
				lis_log_error("out of memory");
				err = LIS_ERR_NO_MEM;
				goto error;
			}
			memcpy((*out_dev_descs)[nb_devs], devs[i][j], sizeof(struct lis_device_descriptor));
			if (asprintf(&((*out_dev_descs)[nb_devs])->dev_id, "%s:%s",
					((*out_dev_descs)[nb_devs])->impl->base_name,
					((*out_dev_descs)[nb_devs])->dev_id) < 0) {
				((*out_dev_descs)[nb_devs])->dev_id = NULL;
				lis_log_error("out of memory");
				err = LIS_ERR_NO_MEM;
				goto error;
			}
			nb_devs++;
		}
	}

	lis_multi_cleanup_dev_descs(private->merged_devs);
	private->merged_devs = *out_dev_descs;
	return LIS_OK;

error:
	lis_multi_cleanup_dev_descs(*out_dev_descs);
	return err;
}

static enum lis_error lis_multi_get_device(struct lis_api *impl, const char *dev_id, struct lis_item **item)
{
	struct lis_multi *private = LIS_MULTI_PRIVATE(impl);
	char *api_name;
	char *sep;
	int i;

	sep = strchr(dev_id, ':');
	if (sep == NULL) {
		lis_log_error("Invalid device id: %s (missing separator ':')", dev_id);
		return LIS_ERR_INVALID_VALUE;
	}

	api_name = strndup(dev_id, sep - dev_id);
	impl = NULL;
	for (i = 0 ; i < private->nb_impls ; i++) {
		if (strcmp(api_name, private->impls[i]->base_name) == 0) {
			impl = private->impls[i];
		}
	}
	if (impl == NULL) {
		lis_log_error("Unknown API: %s", api_name);
		free(api_name);
		return LIS_ERR_INVALID_VALUE;
	}
	free(api_name);

	return impl->get_device(impl, sep + 1, item);
}
