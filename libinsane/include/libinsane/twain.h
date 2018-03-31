#ifndef __LIBINSANE_TWAIN_H
#define __LIBINSANE_TWAIN_H

#include "capi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Wraps the TWAIN API into the LibInsane API.
 *
 * Windows Only.
 * It makes as few adjustments as possible (it's the jobs of the normalizers and the workarounds).
 */
extern enum lis_error lis_api_twain(struct lis_api **api);

#ifdef __cplusplus
}
#endif

#endif
