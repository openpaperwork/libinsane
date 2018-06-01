#ifndef __LIBINSANE_GOBJECT_CAPABILITY_H
#define __LIBINSANE_GOBJECT_CAPABILITY_H

/**
 * LibinsaneCapability:
 * Not actually an enum but a bit field.
 */
typedef enum
{
	LIBINSANE_CAPABILITY_NONE = 0,
	LIBINSANE_CAPABILITY_EMULATED = 1 << 0,
	LIBINSANE_CAPABILITY_AUTOMATIC = 1 << 1,
	LIBINSANE_CAPABILITY_HW_SELECT = 1 << 2,
	LIBINSANE_CAPABILITY_SW_SELECT = 1 << 3,
} LibinsaneCapability;

#endif
