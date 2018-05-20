#include <stdio.h>

#include <CUnit/Basic.h>

#include <libinsane/capi.h>
#include <libinsane/dumb.h>
#include <libinsane/multiplexer.h>
#include <libinsane/util.h>

struct lis_api *g_dumbs[2];
struct lis_api *g_multiplexer;

static int tests_multiplexer_init(void)
{
	enum lis_error err;
	int ret;

	err = lis_api_dumb(&g_dumbs[0], "dummy0");
	ret = LIS_IS_OK(err);
	if (!ret)
		goto end;
	lis_dumb_set_nb_devices(g_dumbs[0], 1);

	err = lis_api_dumb(&g_dumbs[1], "dummy1");
	ret = LIS_IS_OK(err);
	if (!ret)
		goto end;
	lis_dumb_set_nb_devices(g_dumbs[1], 2);

	err = lis_api_multiplexer(g_dumbs, LIS_COUNT_OF(g_dumbs), &g_multiplexer);
	ret = LIS_IS_OK(err);

end:
	return ret;
}

static int tests_multiplexer_clean(void)
{
	g_multiplexer->cleanup(g_multiplexer);
	return 1;
}

static void test_list_devices_prefix(void)
{
	struct lis_device_descriptor **descs;
	enum lis_error err;

	err = g_multiplexer->list_devices(
		g_multiplexer, LIS_DEVICE_LOCATIONS_ANY, &descs
	);
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

	CU_ASSERT_EQUAL(descs[3], NULL);
}

static void test_list_devices_ko(void)
{
	struct lis_device_descriptor **descs;
	enum lis_error err;

	/* if one of the base APIs still works, multiplexer should still work */
	lis_dumb_set_list_devices_return(g_dumbs[1], LIS_ERR_IO_ERROR);
	err = g_multiplexer->list_devices(
		g_multiplexer, LIS_DEVICE_LOCATIONS_ANY, &descs
	);
	CU_ASSERT_TRUE(LIS_IS_OK(err));

	CU_ASSERT_EQUAL(strncmp(descs[0]->dev_id, "dummy0:", 7), 0);
	CU_ASSERT_NOT_EQUAL(descs[0]->impl, NULL);
	CU_ASSERT_NOT_EQUAL(descs[0]->vendor, NULL);
	CU_ASSERT_NOT_EQUAL(descs[0]->model, NULL);

	CU_ASSERT_EQUAL(descs[1], NULL);

	/* if none work, then multiplexer shouln't work */
	lis_dumb_set_list_devices_return(g_dumbs[0], LIS_ERR_JAMMED);
	err = g_multiplexer->list_devices(
		g_multiplexer, LIS_DEVICE_LOCATIONS_ANY, &descs
	);
	CU_ASSERT_TRUE(LIS_IS_ERROR(err));
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

	suite = CU_add_suite("Multiplexer", tests_multiplexer_init, tests_multiplexer_clean);
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
