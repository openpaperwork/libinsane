#ifndef __LIBINSANE_GOBJECT_LOG_H
#define __LIBINSANE_GOBJECT_LOG_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum {
	LIS_LOG_LVL_DEBUG,
	LIS_LOG_LVL_INFO,
	LIS_LOG_LVL_WARNING,
	LIS_LOG_LVL_ERROR,
} LibinsaneLogLevel;

#define LIBINSANE_LOGGER_TYPE libinsane_logger_get_type()
G_DECLARE_INTERFACE(LibinsaneLogger, libinsane_logger, LIBINSANE, LOGGER, GObject)

struct _LibinsaneLoggerInterface
{
	GTypeInterface parent;
	void (*log)(LibinsaneLogger *self, LibinsaneLogLevel lvl, const char *msg);
};

void libinsane_logger_log(LibinsaneLogger *self, LibinsaneLogLevel lvl, const char *msg);


void libinsane_register_logger(LibinsaneLoggerInterface *logger);
void libinsane_unregister_logger();

G_END_DECLS

#endif
