#ifndef __LIBINSANE_STR2IMPL_H
#define __LIBINSANE_STR2IMPL_H

#include "capi.h"
#include "error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Converts a string into a set of implementations.
 *
 * Useful for testing.
 *
 * Supported strings for base APIs:
 * * \ref lis_api_dumb "dumb" (all OS)
 * * \ref lis_api_sane "sane" (Linux only)
 * * \ref lis_api_twain "twain" (Windows only)
 * * \ref lis_api_wia_automation "wia_automation" (Windows only)
 * * \ref lis_api_wia_ll "wia_ll" (Windows only)
 *
 * Support strings for wrappers:
 * * behavior normalizers:
 *   * \ref lis_api_normalizer_all_opts_on_all_sources "all_opts_on_all_sources"
 *   * \ref lis_api_normalizer_flatbed_and_feeder_behavior "flatbed_and_feeder_behavior"
 *   * \ref lis_api_normalizer_min_one_source "min_one_source"
 *   * \ref lis_api_normalizer_raw "raw"
 *   * \ref lis_api_normalizer_raw24 "raw24"
 *   * \ref lis_api_normalizer_resolution_constraint_format "resolution_constraint_format"
 *   * \ref lis_api_normalizer_scan_area_opts "scan_area_opts"
 *   * \ref lis_api_normalizer_source_nodes "source_nodes"
 *   * \ref lis_api_normalizer_source_types "source_types"
 *   * \ref lis_api_normalizer_strip_non_scanners "strip_non_scanners"
 * * workarounds:
 *   * \ref lis_api_workaround_clean_dev_model_char "clean_dev_model_char"
 *   * \ref lis_api_workaround_clean_dev_model_from_manufacturer "clean_dev_model_from_manufacturer"
 *   * \ref lis_api_workaround_dedicated_thread "dedicated_thread"
 *   * \ref lis_api_workaround_no_read_on_inactive_opt "no_read_on_inactive_opt"
 *   * \ref lis_api_workaround_no_write_on_readonly_opt "no_write_on_readonly_opt"
 *   * \ref lis_api_workaround_opt_doc_source "opt_doc_source"
 *   * \ref lis_api_workaround_opt_mode "opt_mode"
 *   * \ref lis_api_workaround_opt_scan_resolution "opt_scan_resolution"
 *   * \ref lis_api_workaround_opts_page_size "opts_page_size"
 *   * \ref lis_api_workaround_strip_translations "strip_translations"
 *
 * API multiplexer is not supported.
 *
 * \param[in] list_of_impls Format: \<base API\>[,\<wrapper\>[,\<wrapper\>[(...)]]]
 * \param[out] impls Requested implementations
 */
extern enum lis_error lis_str2impls(const char *list_of_impls, struct lis_api **impls);

#ifdef __cplusplus
}
#endif

#endif
