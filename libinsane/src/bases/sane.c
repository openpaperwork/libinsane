#include <sane/sane.h>
#include <sane/saneopts.h>

#include <libinsane/capi.h>
#include <libinsane/sane.h>
#include <libinsane/util.h>


enum lis_error lis_api_sane(struct lis_api **impl)
{
	LIS_UNUSED(impl);
	return LIS_ERR_INTERNAL_NOT_IMPLEMENTED;
}
