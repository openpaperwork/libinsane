#ifndef __LIBINSANE_UTIL_H
#define __LIBINSANE_UTIL_H

#include "capi.h"
#include "error.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
 * \brief Copy a value.
 * You *must* \ref lis_free "free the copy" once you're done with it.
 * \param[in] type define the value type.
 * \param[in] original value to copy.
 * \param[out] copy duplicated value.
 */
void lis_copy(
	const enum lis_value_type type, const union lis_value *original, union lis_value *copy
);


/*!
 * \brief Free a copied value.
 * \param[in] type define a the value type.
 * \param[in] value value to free. Do not use it after calling this function.
 */
void lis_free(const enum lis_value_type type, union lis_value *value);


/*!
 * \brief helper to set quickly an option
 */
enum lis_error lis_set_option(struct lis_item *item, const char *opt_name, const char *opt_value);


/*!
 * \brief set the scan area to the maximum that the device can do
 */
enum lis_error lis_set_maximum_scan_area(struct lis_item *item);

#ifdef __cplusplus
}
#endif

#endif
