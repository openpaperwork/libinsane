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
 */
extern enum lis_error lis_api_dumb(struct lis_api **impl);

/*!
 * \brief Dumb implementation. Returns nb_devices scanners.
 * Useful for mocking in tests
 *
 * \param[out] impl will point to the Dumb implementation
 * \param[in] name name of the API (ex: "dumb2")
 * \param[in] nb_devices number of devices that the API must return
 */
extern enum lis_error lis_api_dumb_named_devices(struct lis_api **impl, const char *name, int nb_devices);

#ifdef __cplusplus
}
#endif

#endif
