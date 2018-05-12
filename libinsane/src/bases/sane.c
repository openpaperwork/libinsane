#include <stdlib.h>
#include <string.h>

#include <sane/sane.h>
#include <sane/saneopts.h>

#include <libinsane/capi.h>
#include <libinsane/log.h>
#include <libinsane/sane.h>
#include <libinsane/util.h>


struct lis_sane
{
	struct lis_api parent;
};
#define LIS_SANE_PRIVATE(impl) ((struct lis_sane *)(impl))


static void lis_sane_cleanup(struct lis_api *impl);
static enum lis_error lis_sane_get_devices(struct lis_api *impl, struct lis_device_descriptor ***dev_infos);
static enum lis_error lis_sane_dev_get(struct lis_api *impl, const char *dev_id, struct lis_item **item);


/* ref counter */
static int g_sane_initialized = 0;


static struct lis_api g_sane_impl_template = {
	.cleanup = lis_sane_cleanup,
	.get_devices = lis_sane_get_devices,
	.dev_get = lis_sane_dev_get,
};


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
	struct lis_sane *impl;

	impl = calloc(1, sizeof(struct lis_sane));
	if (impl == NULL) {
		lis_log_error("Out of memory");
		return LIS_ERR_NO_MEM;
	}

	if (g_sane_initialized <= 0) {
		lis_log_debug("sane_init() ...");
		err = sane_status_to_lis_error(sane_init(&version_code, auth_callback));
		lis_log_debug("sane_init(): 0x%X, %s", err, lis_strerror(err));
		if (LIS_IS_ERROR(err)) {
			free(impl);
			return err;
		}
		lis_log_info("Sane version code: 0x%X", version_code);
		g_sane_initialized++;
	}

	memcpy(&impl->parent, &g_sane_impl_template, sizeof(impl->parent));
	*out_impl = &impl->parent;
	return LIS_OK;
}

static void lis_sane_cleanup(struct lis_api *impl)
{
	if (g_sane_initialized > 0) {
		lis_log_debug("sane_exit()");
		sane_exit();
		g_sane_initialized--;
	}
	free(impl);
}

static enum lis_error lis_sane_get_devices(struct lis_api *impl, struct lis_device_descriptor ***dev_infos)
{
	/* TODO */
	return LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
}

static enum lis_error lis_sane_dev_get(struct lis_api *impl, const char *dev_id, struct lis_item **item)
{
	/* TODO */
	return LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
}
