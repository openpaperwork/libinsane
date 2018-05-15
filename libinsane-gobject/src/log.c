#include <stdio.h>

#include <libinsane/log.h>
#include <libinsane-gobject/log.h>

G_DEFINE_INTERFACE(LibinsaneLogger, libinsane_logger, G_TYPE_OBJECT)


static LibinsaneLogger *g_logger = NULL;
static LibinsaneLoggerInterface *g_logger_iface = NULL;

static lis_log_callback log_callback;

static struct lis_log_callbacks g_callbacks = {
	.callbacks = {
		[LIS_LOG_LVL_DEBUG] = log_callback,
		[LIS_LOG_LVL_INFO] = log_callback,
		[LIS_LOG_LVL_WARNING] = log_callback,
		[LIS_LOG_LVL_ERROR] = log_callback,
	}
};


static void libinsane_logger_default_init(LibinsaneLoggerInterface *iface)
{

}


void libinsane_logger_log(LibinsaneLogger *self, LibinsaneLogLevel lvl, const char *msg)
{
	LibinsaneLoggerInterface *iface;

	g_return_if_fail(LIBINSANE_IS_LOGGER(self));

	iface = LIBINSANE_LOGGER_GET_IFACE(self);
	iface->log(self, lvl, msg);
}

static void log_callback(enum lis_log_level lis_lvl, const char *msg)
{
	LibinsaneLogLevel lvl = LIBINSANE_LOG_LEVEL_ERROR;

	if (g_logger_iface == NULL || g_logger_iface->log == NULL) {
		fprintf(stderr, "Warning: no log callback defined (iface ? %d). Defaulting to stderr\n",
				(g_logger_iface != NULL));
		fprintf(stderr, "%d: %s\n", lis_lvl, msg);
		return;
	}

	switch(lis_lvl)
	{
		case LIS_LOG_LVL_DEBUG:
			lvl = LIBINSANE_LOG_LEVEL_DEBUG;
			break;
		case LIS_LOG_LVL_INFO:
			lvl = LIBINSANE_LOG_LEVEL_INFO;
			break;
		case LIS_LOG_LVL_WARNING:
			lvl = LIBINSANE_LOG_LEVEL_WARNING;
			break;
		case LIS_LOG_LVL_ERROR:
			lvl = LIBINSANE_LOG_LEVEL_ERROR;
			break;
	}

	g_logger_iface->log(g_logger, lvl, msg);
}

void libinsane_register_logger(LibinsaneLogger *logger)
{
	if (logger != NULL) {
		g_object_ref(logger);

		g_logger = logger;
		g_logger_iface = LIBINSANE_LOGGER_GET_IFACE(logger);

		if (g_logger_iface == NULL) {
			fprintf(stderr, "[WARNING] Expected Logger object !\n");
		} else {
			lis_set_log_callbacks(&g_callbacks);
		}
	} else {
		if (g_logger != NULL) {
			libinsane_unregister_logger();
		}
		g_logger = NULL;
		g_logger_iface = NULL;
	}
}


void libinsane_unregister_logger()
{
	if (g_logger != NULL) {
		g_object_unref(g_logger);
	}
	g_logger = NULL;
	g_logger_iface = NULL;
}
