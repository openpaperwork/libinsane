#include <stdlib.h>
#include <string.h>

#include <libinsane/capi.h>
#include <libinsane/dumb.h>
#include <libinsane/error.h>
#include <libinsane/log.h>
#include <libinsane/normalizers.h>
#include <libinsane/str2impls.h>
#include <libinsane/util.h>
#include <libinsane/workarounds.h>

#ifdef OS_LINUX
#include <libinsane/sane.h>
#endif

#ifdef OS_WINDOWS
#include <libinsane/twain.h>
#include <libinsane/wia_automation.h>
#include <libinsane/wia_ll.h>
#endif

enum lis_error lis_str2impls(const char *list_of_impls, struct lis_api **impls)
{
	enum lis_error err = LIS_OK;
	char *input_str;
	char *save_ptr = NULL;
	const char *tok;
	struct lis_api *next;

	lis_log_debug("enter");

	input_str = strdup(list_of_impls);
	if (input_str == NULL) {
		lis_log_debug("error no mem");
		return LIS_ERR_NO_MEM;
	}

	*impls = NULL;

	for (tok = strtok_r(input_str, ",", &save_ptr) ;
			tok != NULL ;
			tok = strtok_r(NULL, ",", &save_ptr)) {
		if (*impls == NULL) {

			// look for a base API
			if (strcmp(tok, "dumb") == 0) {
				err = lis_api_dumb(&next, "dumb");
#ifdef OS_LINUX
			} else if (strcmp(tok, "sane") == 0) {
				err = lis_api_sane(&next);
#endif
#ifdef OS_WINDOWS
			} else if (strcmp(tok, "twain") == 0) {
				err = lis_api_twain(&next);
			} else if (strcmp(tok, "wia_automation") == 0) {
				err = lis_api_wia_automation(&next);
			} else if (strcmp(tok, "wia_ll") == 0) {
				err = lis_api_wia_ll(&next);
#endif
			} else {
				lis_log_error("Unknown base API: %s", tok);
				err = LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
				goto error;
			}

		} else {

			// look for a wrapper
			// -> normalizers
			if (strcmp(tok, "all_opts_on_all_sources") == 0) {
				err = lis_api_normalizer_all_opts_on_all_sources(*impls, &next);
			} else if (strcmp(tok, "flatbed_and_feeder_behavior") == 0) {
				err = lis_api_normalizer_flatbed_and_feeder_behavior(*impls, &next);
			} else if (strcmp(tok, "min_one_source") == 0) {
				err = lis_api_normalizer_min_one_source(*impls, &next);
			} else if (strcmp(tok, "raw") == 0) {
				err = lis_api_normalizer_raw(*impls, &next);
			} else if (strcmp(tok, "raw24") == 0) {
				err = lis_api_normalizer_raw24(*impls, &next);
			} else if (strcmp(tok, "resolution") == 0) {
				err = lis_api_normalizer_resolution_format(*impls, &next);
			} else if (strcmp(tok, "scan_area_opts") == 0) {
				err = lis_api_normalizer_scan_area_opts(*impls, &next);
			} else if (strcmp(tok, "source_nodes") == 0) {
				err = lis_api_normalizer_source_nodes(*impls, &next);
			} else if (strcmp(tok, "source_types") == 0) {
				err = lis_api_normalizer_source_types(*impls, &next);
			} else if (strcmp(tok, "strip_non_scanners") == 0) {
				err = lis_api_normalizer_strip_non_scanners(*impls, &next);
			}
			// -> workarounds
			else if (strcmp(tok, "clean_dev_model_char") == 0) {
				err = lis_api_workaround_clean_dev_model_char(*impls, &next);
			} else if (strcmp(tok, "clean_dev_model_from_manufacturer") == 0) {
				err = lis_api_workaround_clean_dev_model_from_manufacturer(*impls, &next);
			} else if (strcmp(tok, "dedicated_thread") == 0) {
				err = lis_api_workaround_dedicated_thread(*impls, &next);
			} else if (strcmp(tok, "no_read_on_inactive_opt") == 0) {
				err = lis_api_workaround_no_read_on_inactive_opt(*impls, &next);
			} else if (strcmp(tok, "no_write_on_readonly_opt") == 0) {
				err = lis_api_workaround_no_write_on_readonly_opt(*impls, &next);
			} else if (strcmp(tok, "opt_doc_source") == 0) {
				err = lis_api_workaround_opt_doc_source(*impls, &next);
			} else if (strcmp(tok, "opt_mode") == 0) {
				err = lis_api_workaround_opt_mode(*impls, &next);
			} else if (strcmp(tok, "opt_scan_resolution") == 0) {
				err = lis_api_workaround_opt_scan_resolution(*impls, &next);
			} else if (strcmp(tok, "opts_page_size") == 0) {
				err = lis_api_workaround_opts_page_size(*impls, &next);
			} else if (strcmp(tok, "strip_translations") == 0) {
				err = lis_api_workaround_strip_translations(*impls, &next);
			} else {
				lis_log_error("Unknown API wrapper: %s", tok);
				err = LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
				goto error;
			}
		}

		if (LIS_IS_ERROR(err)) {
			lis_log_error("Failed to instanciate API implementation '%s'", tok);
			goto error;
		}

		*impls = next;
	}

	free(input_str);
	lis_log_debug("leave");
	return LIS_OK;

error:
	lis_log_debug("error");
	if (*impls != NULL) {
		(*impls)->cleanup(*impls);
	}
	free(input_str);
	return err;
}
