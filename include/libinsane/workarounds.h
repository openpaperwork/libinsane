#ifndef __LIBINSANE_WORKAROUNDS_H
#define __LIBINSANE_WORKAROUNDS_H

#include "capi.h"


#ifdef __cplusplus
extern "C" {
#endif

extern enum lis_error lis_api_workaround_opt_scan_resolution(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_opt_doc_source(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_opt_mode_values(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_opts_page_size(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_strip_translations(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_clean_dev_model_from_manufacturer(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_clean_dev_model_char(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_no_read_access_to_inactive_opt(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_no_write_on_readonly_opt(
	struct lis_api *to_wrap, struct lis_api **api
);
extern enum lis_error lis_api_workaround_dedicated_thread(
	struct lis_api *to_wrap, struct lis_api **api
);

#ifdef __cplusplus
}
#endif

#endif
