#include <stdio.h>

#include <libinsane/log.h>
#include <libinsane-gobject/log.h>

G_DEFINE_INTERFACE(LibinsaneLogger, libinsane_logger, G_TYPE_OBJECT)


static LibinsaneLogger *g_logger = NULL;

static lis_log_callback log_callback;
static void libinsane_logger_real_log(LibinsaneLogger *self, LibinsaneLogLevel lvl, const char *msg);

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
	iface->log = libinsane_logger_real_log;
}

static void libinsane_logger_real_log(LibinsaneLogger *self, LibinsaneLogLevel lvl, const char *msg)
{
	fprintf(stderr, "[missing log callback] %d: %s\n", lvl, msg);
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

	if (g_logger == NULL) {
		fprintf(stderr, "Warning: no log callback defined. Defaulting to stderr\n");
		fprintf(stderr, "[missing logger object] %d: %s\n", lis_lvl, msg);
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

	libinsane_logger_log(g_logger, lvl, msg);
}

void libinsane_register_logger(LibinsaneLogger *logger)
{
	if (logger != NULL) {
		g_object_ref(logger);

		g_logger = logger;
		lis_set_log_callbacks(&g_callbacks);
	} else {
		libinsane_unregister_logger();
	}
}


void libinsane_unregister_logger()
{
	if (g_logger != NULL) {
		g_object_unref(g_logger);
	}
	g_logger = NULL;
}
