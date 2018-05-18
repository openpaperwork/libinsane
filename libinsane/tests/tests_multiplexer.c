#include <stdio.h>

#include <CUnit/Basic.h>

static void test_list_devices_prefix(void)
{

}

static void test_get_device_ok(void)
{

}

static void test_get_device_ko(void)
{

}

static int register_tests(void)
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("Multiplexer", NULL, NULL);
	if (suite == NULL) {
		fprintf(stderr, "CU_add_suite() failed\n");
		return 0;
	}

	if (CU_add_test(suite, "list_devices() prefix", test_list_devices_prefix) == NULL
			|| CU_add_test(suite, "get_device() ok", test_get_device_ok) == NULL
			|| CU_add_test(suite, "get_device() ko", test_get_device_ko) == NULL) {
		fprintf(stderr, "CU_add_test() has failed\n");
		return 0;
	}

	return 1;
}

int main(int argc, char **argv)
{
	if (CU_initialize_registry() != CUE_SUCCESS) {
		fprintf(stderr, "CU_initialize_registry() failed\n");
		return CU_get_error();
	}

	if (!register_tests()) {
		fprintf(stderr, "registring tests has failed\n");
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

