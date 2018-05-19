#ifndef __LIBINSANE_DUMB_H
#define __LIBINSANE_DUMB_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Dumb implementation. Returns 0 scanners. Only useful for testing.
 * \param[out] impl will point to the Dumb implementation
 * \param[in] name name of the API
 */
extern enum lis_error lis_api_dumb(struct lis_api **impl, const char *name);

void lis_dumb_set_nb_devices(struct lis_api *impl, int nb_devices);

#ifdef __cplusplus
}
#endif

#endif
