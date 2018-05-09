#ifndef __LIBINSANE_GOBJECT_VALUE_TYPE_H
#define __LIBINSANE_GOBJECT_VALUE_TYPE_H

typedef enum
{
	LIBINSANE_VALUE_TYPE_BOOL, /* int: 0 (false) or 1 (true) */
	LIBINSANE_VALUE_TYPE_INTEGER, /* int */
	LIBINSANE_VALUE_TYPE_FLOAT, /* float */
	LIBINSANE_VALUE_TYPE_STRING, /* (char *) */
	LIBINSANE_VALUE_TYPE_IMAGE_FORMAT /* Image format. See #LibinsaneImgFormat . */
} LibinsaneValueType;

#endif
