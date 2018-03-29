#ifndef __LIBINSANE_MULTIPLEXER_H
#define __LIBINSANE_MULTIPLEXER_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief API multiplexer
 *
 * LibInsane must be able to handle many APIs at once. For instance, on Windows,
 * it must be able to use WIA2 (low-level and automation) and TWAIN, at the same
 * time. So it includes an API multiplexer.
 *
 * The API multiplexer takes many API implementations in input, and merge them
 * to present them as a single one.
 *
 * \param[in] input_implementations Implementations to wrap together.
 * \param[in] nb_input_implementations Number of implementations to wrap together.
 * \param[out] output_implementation Implementation wrapping all the input implementations.
 */
extern enum lis_error lis_api_multiplexer(
	struct lis_api **input_implementations, int nb_input_implementations,
	struct lis_api **output_implementation
);

#ifdef __cplusplus
}
#endif

#endif
