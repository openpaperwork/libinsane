#ifndef __LIBINSANE_DUMB_H
#define __LIBINSANE_DUMB_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Dumb implementation. Returns 0 scanners. Only useful for testing.
 * Used mostly in unit tests.
 * \param[out] impl will point to the Dumb implementation
 * \param[in] name name of the API
 */
extern enum lis_error lis_api_dumb(struct lis_api **impl, const char *name);

void lis_dumb_set_nb_devices(struct lis_api *self, int nb_devices);
void lis_dumb_set_list_devices_return(struct lis_api *self, enum lis_error ret);
void lis_dumb_set_get_device_return(struct lis_api *self, enum lis_error ret);

#ifdef __cplusplus
}
#endif

#endif
