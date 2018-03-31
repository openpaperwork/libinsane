#ifndef __LIBINSANE_SANE_H
#define __LIBINSANE_SANE_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Wraps the Sane API into the LibInsane API.
 *
 * Linux Only.
 * It makes as few adjustments as possible (it's the jobs of the normalizers and the workarounds).
 */
extern enum lis_error lis_api_sane(struct lis_api **api);

#ifdef __cplusplus
}
#endif

#endif
