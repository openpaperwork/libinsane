#ifndef __LIBINSANE_WIA_LL_H
#define __LIBINSANE_WIA_LL_H

#include "capi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Wraps the WIA low-level API into the LibInsane API.
 *
 * Windows Only.
 * It makes as few adjustments as possible (it's the jobs of the normalizers and the workarounds).
 */
extern enum lis_error lis_api_wia_ll(struct lis_api **impl);

#ifdef __cplusplus
}
#endif

#endif
