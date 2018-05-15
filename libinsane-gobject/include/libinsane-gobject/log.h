#ifndef __LIBINSANE_GOBJECT_LOG_H
#define __LIBINSANE_GOBJECT_LOG_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum {
	LIBINSANE_LOG_LEVEL_DEBUG,
	LIBINSANE_LOG_LEVEL_INFO,
	LIBINSANE_LOG_LEVEL_WARNING,
	LIBINSANE_LOG_LEVEL_ERROR,
} LibinsaneLogLevel;

#define LIBINSANE_TYPE_LOGGER libinsane_logger_get_type()
G_DECLARE_INTERFACE(LibinsaneLogger, libinsane_logger, LIBINSANE, LOGGER, GObject)

struct _LibinsaneLoggerInterface
{
	GTypeInterface parent;
	void (*log)(LibinsaneLogger *self, LibinsaneLogLevel lvl, const char *msg);
};

void libinsane_logger_log(LibinsaneLogger *self, LibinsaneLogLevel lvl, const char *msg);


void libinsane_register_logger(LibinsaneLogger *logger);
void libinsane_unregister_logger();

G_END_DECLS

#endif
