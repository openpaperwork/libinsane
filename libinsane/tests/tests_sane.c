#include <stdio.h>

#include <CUnit/Basic.h>

#include <libinsane/capi.h>
#include <libinsane/dumb.h>
#include <libinsane/sane.h>
#include <libinsane/util.h>

#include "util.h"

#define TEST_DEV_ID "test:0"


static struct lis_api *g_sane;
static struct lis_item *g_test_device;


static int tests_sane_init(void)
{
	enum lis_error err;

	err = lis_api_sane(&g_sane);
	if (LIS_IS_ERROR(err)) {
		fprintf(stderr, "Tests init: Failed to init Sane: 0x%X, %s\n", err, lis_strerror(err));
		return -1;
	}
	err = g_sane->get_device(g_sane, TEST_DEV_ID, &g_test_device);
	if (LIS_IS_ERROR(err)) {
		fprintf(stderr, "Tests init: Failed to get test device: 0x%X, %s\n", err, lis_strerror(err));
		return -1;
	}

	return 0;
}


static int tests_sane_clean(void)
{
	g_sane->cleanup(g_sane);
	return 0;
}


static void tests_sane_list_devices(void)
{
	struct lis_device_descriptor **devs;
	enum lis_error err;
	int i;
	int has_test0 = 0;

	err = g_sane->list_devices(g_sane, LIS_DEVICE_LOCATIONS_LOCAL_ONLY, &devs);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));

	for (i = 0 ; devs[i] != NULL ; i++) {
		LIS_ASSERT_NOT_EQUAL(devs[i]->dev_id, NULL);
		if (devs[i]->dev_id != NULL && strcmp(devs[i]->dev_id, TEST_DEV_ID) == 0) {
			has_test0 = 1;
		}
		LIS_ASSERT_NOT_EQUAL(devs[i]->vendor, NULL);
		LIS_ASSERT_NOT_EQUAL(devs[i]->model, NULL);
		LIS_ASSERT_NOT_EQUAL(devs[i]->type, NULL);
	}
	LIS_ASSERT_TRUE(has_test0);
}

int register_tests(void)
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("Sane", tests_sane_init, tests_sane_clean);
	if (suite == NULL) {
		fprintf(stderr, "CU_add_suite() failed\n");
		return 0;
	}

	if (CU_add_test(suite, "list_devices()", tests_sane_list_devices) == NULL) {
		fprintf(stderr, "CU_add_test() has failed\n");
		return 0;
	}

	return 1;
}
