#ifndef __LIBINSANE_SANE_H
#define __LIBINSANE_SANE_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

extern enum lis_error lis_api_sane(struct lis_api **api);

#ifdef __cplusplus
}
#endif

#endif
