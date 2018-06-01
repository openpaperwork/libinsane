#ifndef __LIBINSANE_LOG_H
#define __LIBINSANE_LOG_H

enum lis_log_level {
	LIS_LOG_LVL_MIN = 0,
	LIS_LOG_LVL_DEBUG = LIS_LOG_LVL_MIN,
	LIS_LOG_LVL_INFO,
	LIS_LOG_LVL_WARNING,
	LIS_LOG_LVL_ERROR,
	LIS_LOG_LVL_MAX = LIS_LOG_LVL_ERROR,
};


/*!
 * \brief log callback
 * \param[in] msg does no include "\n".
 */
typedef void (lis_log_callback)(enum lis_log_level lvl, const char *msg);


struct lis_log_callbacks {
	lis_log_callback *callbacks[LIS_LOG_LVL_MAX + 1];
};


/*!
 * \brief register callbacks that will be used to get back messages.
 * Only one set of callback can be registered at one time.
 * Calling this function will unset previously set callbacks.
 * By default, all log messages go to stderr.
 * \param[in] callbacks callback to use. Pointer must remain valid until lis_set_log_callbacks is
 *		called again. NULL will reset callbacks to their default (stderr).
 */
extern void lis_set_log_callbacks(const struct lis_log_callbacks *callbacks);


/* functions inside libinsane to log */

extern void lis_log(enum lis_log_level lvl, const char *file, int line, const char *func, const char *fmt, ...)
#ifdef __GNUC__
	__attribute__ ((format (printf, 5, 6)))
#endif
	;

#define lis_log_debug(...) lis_log(LIS_LOG_LVL_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__);
#define lis_log_info(...) lis_log(LIS_LOG_LVL_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__);
#define lis_log_warning(...) lis_log(LIS_LOG_LVL_WARNING, __FILE__, __LINE__, __func__, __VA_ARGS__);
#define lis_log_error(...) lis_log(LIS_LOG_LVL_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__);

/*!
 * \brief default log callback provided for convenience.
 * Will log to stderr.
 */
lis_log_callback lis_log_stderr;


#endif
