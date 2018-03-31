#ifndef __LIBINSANE_WIA_AUTOMATION_H
#define __LIBINSANE_WIA_AUTOMATION_H

#include "capi.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Wraps the WIA automation layer into the LibInsane API.
 *
 * Windows Only.
 * It makes as few adjustments as possible (it's the jobs of the normalizers and the workarounds).
 */
extern enum lis_error lis_api_wia_automation(struct lis_api **api);

#ifdef __cplusplus
}
#endif

#endif
