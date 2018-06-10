#ifndef __LIBINSANE_SCAN_SESSION_PRIVATE_H
#define __LIBINSANE_SCAN_SESSION_PRIVATE_H

#include <libinsane/capi.h>

#include "scan_session.h"

LibinsaneScanSession *libinsane_scan_session_new_from_libinsane(
	struct lis_scan_session *scan_session
);

#endif
