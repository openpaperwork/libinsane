#include <libinsane-gobject/log.h>

G_DEFINE_INTERFACE(LibinsaneLogger, libinsane_logger, G_TYPE_OBJECT)


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

void libinsane_register_logger(LibinsaneLoggerInterface *logger)
{
	/* TODO */
}


void libinsane_unregister_logger()
{
	/* TODO */
}
