#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <libinsane/log.h>


static lis_log_callback log_debug_stderr;
static lis_log_callback log_info_stderr;
static lis_log_callback log_warning_stderr;
static lis_log_callback log_error_stderr;


const struct lis_log_callbacks g_default_callbacks = {
	.callbacks = {
		[LIS_LOG_LVL_DEBUG] = log_debug_stderr,
		[LIS_LOG_LVL_INFO] = log_info_stderr,
		[LIS_LOG_LVL_WARNING] = log_warning_stderr,
		[LIS_LOG_LVL_ERROR] = log_error_stderr,
	}
};


const struct lis_log_callbacks *g_current_callbacks = &g_default_callbacks;


void lis_set_log_callbacks(const struct lis_log_callbacks *callbacks)
{
	if (callbacks == NULL) {
		callbacks = &g_default_callbacks;
	}
	g_current_callbacks = callbacks;
}


static void log_debug_stderr(const char *msg)
{
	fprintf(stderr, "[LibInsane:DEBUG] %s\n", msg);
}


static void log_info_stderr(const char *msg)
{
	fprintf(stderr, "[LibInsane:INFO] %s\n", msg);
}


static void log_warning_stderr(const char *msg)
{
	fprintf(stderr, "[LibInsane:WARNING] %s\n", msg);
}


static void log_error_stderr(const char *msg)
{
	fprintf(stderr, "[LibInsane:ERROR] %s\n", msg);
}


void lis_log(enum lis_log_level lvl, const char *file, int line, const char *msg, ...)
{
	static char g_buffer[2048];
	int r;
	va_list ap;

	assert(lvl >= LIS_LOG_LVL_MIN);
	assert(lvl <= LIS_LOG_LVL_MAX);

	r = snprintf(g_buffer, sizeof(g_buffer), "%s:L%d: ", file, line);

	va_start(ap, msg);
	r = vsnprintf(g_buffer + r, sizeof(g_buffer) - r, msg, ap);
	va_end(ap);

	if (r < 0) {
		fprintf(stderr, "Failed to format log output: %d, %s", errno, strerror(errno));
		return;
	}

	g_current_callbacks->callbacks[lvl](g_buffer);
}
