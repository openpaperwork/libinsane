#include <stdio.h>

#include <CUnit/Basic.h>

#include <libinsane/capi.h>
#include <libinsane/dumb.h>
#include <libinsane/multiplexer.h>
#include <libinsane/util.h>


static void test_list_devices_prefix(void)
{
	struct lis_api *dumbs[2];
	struct lis_api *multiplexer;
	struct lis_device_descriptor **descs;
	enum lis_error err;

	err = lis_api_dumb(&dumbs[0], "dummy0");
	CU_ASSERT_TRUE(LIS_IS_OK(err));
	lis_dumb_set_nb_devices(dumbs[0], 1);

	err = lis_api_dumb(&dumbs[1], "dummy1");
	CU_ASSERT_TRUE(LIS_IS_OK(err));
	lis_dumb_set_nb_devices(dumbs[1], 2);

	err = lis_api_multiplexer(dumbs, LIS_COUNT_OF(dumbs), &multiplexer);
	CU_ASSERT_TRUE(LIS_IS_OK(err));

	err = multiplexer->list_devices(multiplexer, 0 /* !local_only */, &descs);
	CU_ASSERT_TRUE(LIS_IS_OK(err));

	CU_ASSERT_EQUAL(strncmp(descs[0]->dev_id, "dummy0:", 7), 0);
	CU_ASSERT_NOT_EQUAL(descs[0]->impl, NULL);
	CU_ASSERT_NOT_EQUAL(descs[0]->vendor, NULL);
	CU_ASSERT_NOT_EQUAL(descs[0]->model, NULL);

	CU_ASSERT_EQUAL(strncmp(descs[1]->dev_id, "dummy1:", 7), 0);
	CU_ASSERT_NOT_EQUAL(descs[1]->impl, NULL);
	CU_ASSERT_NOT_EQUAL(descs[1]->vendor, NULL);
	CU_ASSERT_NOT_EQUAL(descs[1]->model, NULL);

	CU_ASSERT_EQUAL(strncmp(descs[2]->dev_id, "dummy1:", 7), 0);
	CU_ASSERT_NOT_EQUAL(descs[2]->impl, NULL);
	CU_ASSERT_NOT_EQUAL(descs[2]->vendor, NULL);
	CU_ASSERT_NOT_EQUAL(descs[2]->model, NULL);

	multiplexer->cleanup(multiplexer);
}

static void test_list_devices_ko(void)
{

}

static void test_get_device_ok(void)
{

}

static void test_get_device_not_found(void)
{

}

static void test_get_device_ko(void)
{

}

int register_tests(void)
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("Multiplexer", NULL, NULL);
	if (suite == NULL) {
		fprintf(stderr, "CU_add_suite() failed\n");
		return 0;
	}

	if (CU_add_test(suite, "list_devices() prefix", test_list_devices_prefix) == NULL
			|| CU_add_test(suite, "list_devices() ko", test_list_devices_ko) == NULL
			|| CU_add_test(suite, "get_device() ok", test_get_device_ok) == NULL
			|| CU_add_test(suite, "get_device() not found",
				test_get_device_not_found) == NULL
			|| CU_add_test(suite, "get_device() ko", test_get_device_ko) == NULL) {
		fprintf(stderr, "CU_add_test() has failed\n");
		return 0;
	}

	return 1;
}
