#ifndef __LIBINSANE_CAPI_H
#define __LIBINSANE_CAPI_H

#ifdef __cplusplus
extern "C" {
#endif

struct lis_api {
	struct lis_ctx *(*init)(void);

	void *(*cleanup)(void);
};

#ifdef __cplusplus
}
#endif

#endif
