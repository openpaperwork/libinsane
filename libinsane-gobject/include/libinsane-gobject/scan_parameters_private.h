#ifndef __LIBINSANE_SCAN_PARAMETERS_PRIVATE_H
#define __LIBINSANE_SCAN_PARAMETERS_PRIVATE_H

#include <libinsane/capi.h>

#include "item.h"

LibinsaneScanParameters *libinsane_scan_parameters_new_from_libinsane(struct lis_scan_parameters *params);

#endif
