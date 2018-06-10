#include <stdio.h>
#include <stdlib.h>

#include <CUnit/Basic.h>

#include <libinsane/log.h>


int register_tests(void);

// #define LOG_DEBUG

#ifndef LOG_DEBUG
static void noop() {}
#endif

const struct lis_log_callbacks g_log_callbacks = {
	.callbacks = {
#ifdef LOG_DEBUG
		[LIS_LOG_LVL_DEBUG] = lis_log_stderr,
		[LIS_LOG_LVL_INFO] = lis_log_stderr,
		[LIS_LOG_LVL_WARNING] = lis_log_stderr,
#else
		[LIS_LOG_LVL_DEBUG] = noop,
		[LIS_LOG_LVL_INFO] = noop,
		[LIS_LOG_LVL_WARNING] = noop,
#endif
		[LIS_LOG_LVL_ERROR] = lis_log_stderr,
	}
};

int main(int argc, char **argv)
{
	CU_ErrorCode err;
	int has_failed;

	lis_set_log_callbacks(&g_log_callbacks);

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
	has_failed = CU_get_number_of_failures();
	if (!has_failed && CU_get_number_of_tests_run() == 0) {
		has_failed = 1;
	}
	CU_cleanup_registry();
	err = CU_get_error();
	if (err != 0)
		return err;
	return (has_failed ? EXIT_FAILURE : EXIT_SUCCESS);
}
