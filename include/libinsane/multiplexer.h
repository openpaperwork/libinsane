#ifndef __LIBINSANE_MULTIPLEXER_H
#define __LIBINSANE_MULTIPLEXER_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

extern enum lis_error lis_api_multiplexer(
	struct lis_api **input_apis, int nb_input_apis,
	struct lis_api **output_api
);

#ifdef __cplusplus
}
#endif

#endif
