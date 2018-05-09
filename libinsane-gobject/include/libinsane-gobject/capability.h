#ifndef __LIBINSANE_GOBJECT_CAPABILITY_H
#define __LIBINSANE_GOBJECT_CAPABILITY_H

#include <libinsane/capi.h>

/**
 * LibinsaneCapability:
 * Not actually an enum but a bit field.
 * Just here so values are visible in GObject API.
 */
typedef enum
{
	LIBINSANE_CAPABILITY_EMULATED = LIS_CAP_EMULATED,
	LIBINSANE_CAPABILITY_AUTOMATIC = LIS_CAP_AUTOMATIC,
	LIBINSANE_CAPABILITY_HW_SELECT = LIS_CAP_HW_SELECT,
	LIBINSANE_CAPABILITY_SW_SELECT = LIS_CAP_SW_SELECT,
} LibinsaneCapability;

#endif
