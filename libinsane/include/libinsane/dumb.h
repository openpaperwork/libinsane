#ifndef __LIBINSANE_DUMB_H
#define __LIBINSANE_DUMB_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Dumb implementation. Returns 0 scanners. Only useful for testing.
 */
extern enum lis_error lis_api_dumb(struct lis_api **api);

#ifdef __cplusplus
}
#endif

#endif
