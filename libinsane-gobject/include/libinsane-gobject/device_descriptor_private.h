#ifndef __LIBINSANE_DEVICE_DESCRIPTOR_PRIVATE_H
#define __LIBINSANE_DEVICE_DESCRIPTOR_PRIVATE_H

#include <libinsane/capi.h>

#include "device_descriptor.h"


LibinsaneDeviceDescriptor *libinsane_device_descriptor_new_from_libinsane(
	const struct lis_device_descriptor *desc
);

#endif
