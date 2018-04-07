#ifndef __LIBINSANE_SAFEBET_H
#define __LIBINSANE_SAFEBET_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Get all the API implementations to access all the OS API and
 *		all the workarounds and normalizers and multiplexer that may be useful.
 *
 * This is the function you should use in most application to get an API.
 *
 * This function also check various environment variables, making it possible
 * to disable individual workarounds or normalizers.
 */
extern enum lis_error lis_safebet(struct lis_api **api);

#ifdef __cplusplus
}
#endif

#endif
