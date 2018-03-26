#ifndef __LIBINSANE_NORMALIZERS_H
#define __LIBINSANE_NORMALIZERS_H

#include "capi.h"


#ifdef __cplusplus
extern "C" {
#endif

extern enum lis_error lis_api_normalizer_source_nodes(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_has_source(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_all_opts_on_all_sources(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_scan_area_opts(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_resolution_constraint_format(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_source_types(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_strip_v4l(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_raw(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_raw24(struct lis_api *to_wrap, struct lis_api **api);
extern enum lis_error lis_api_normalizer_flatbed_and_feeder_behavior(struct lis_api *to_wrap, struct lis_api **api);


#ifdef __cplusplus
}
#endif

#endif
