#ifndef __LIBINSANE_GOBJECT_OPTION_DESCRIPTOR_PRIVATE_H
#define __LIBINSANE_GOBJECT_OPTION_DESCRIPTOR_PRIVATE_H

#include <libinsane/capi.h>

#include "option_descriptor.h"

LibinsaneOptionDescriptor *libinsane_option_descriptor_new_from_libinsane(
		struct lis_option_descriptor *desc
	);

#endif
