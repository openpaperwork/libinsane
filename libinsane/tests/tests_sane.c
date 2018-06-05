#include <stdio.h>
#include <string.h>

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
	g_test_device->close(g_test_device);
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


static void tests_sane_get_device_ko(void)
{
	enum lis_error err;
	struct lis_item *item = NULL;

	err = g_sane->get_device(g_sane, "non-existing", &item);
	LIS_ASSERT_EQUAL(err, LIS_ERR_INVALID_VALUE);
	LIS_ASSERT_EQUAL(item, NULL);
}


static void tests_sane_item_get_children(void)
{
	enum lis_error err;
	struct lis_item **children = NULL;

	err = g_test_device->get_children(g_test_device, &children);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_NOT_EQUAL(children, NULL);
	LIS_ASSERT_EQUAL(children[0], NULL);
}


static void tests_sane_get_resolution(void)
{
	enum lis_error err;
	struct lis_option_descriptor **options = NULL;
	union lis_value value;
	int i;

	err = g_test_device->get_options(g_test_device, &options);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_NOT_EQUAL(options, NULL);

	for (i = 0 ; options[i] != NULL ; i++) {
		if (strcmp(options[i]->name, "resolution") == 0) {
			break;
		}
	}
	LIS_ASSERT_NOT_EQUAL(options[i], NULL);

	LIS_ASSERT_NOT_EQUAL(options[i]->title, NULL);
	LIS_ASSERT_NOT_EQUAL(options[i]->desc, NULL);
	LIS_ASSERT_EQUAL(options[i]->capabilities, LIS_CAP_SW_SELECT);
	LIS_ASSERT_EQUAL(options[i]->value.type, LIS_TYPE_DOUBLE);
	LIS_ASSERT_EQUAL(options[i]->value.unit, LIS_UNIT_DPI);
	LIS_ASSERT_EQUAL(options[i]->constraint.type, LIS_CONSTRAINT_RANGE);

	err = options[i]->fn.get_value(options[i], &value);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(value.dbl, 50.0); // check current value
}


static void tests_sane_set_resolution(void)
{
	enum lis_error err;
	struct lis_option_descriptor **options = NULL;
	union lis_value value;
	int i;
	int set_flags;

	err = g_test_device->get_options(g_test_device, &options);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_NOT_EQUAL(options, NULL);

	for (i = 0 ; options[i] != NULL ; i++) {
		if (strcmp(options[i]->name, "resolution") == 0) {
			break;
		}
	}
	LIS_ASSERT_NOT_EQUAL(options[i], NULL);

	LIS_ASSERT_EQUAL(options[i]->value.type, LIS_TYPE_DOUBLE);
	LIS_ASSERT_EQUAL(options[i]->constraint.type, LIS_CONSTRAINT_RANGE);
	LIS_ASSERT_EQUAL(options[i]->constraint.possible.range.min.dbl, 1.0);
	LIS_ASSERT_EQUAL(options[i]->constraint.possible.range.max.dbl, 1200.0);
	LIS_ASSERT_EQUAL(options[i]->constraint.possible.range.interval.dbl, 1.0);

	err = options[i]->fn.get_value(options[i], &value);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(value.dbl, 50.0); // check current value

	value.dbl = 1000.0;
	err = options[i]->fn.set_value(options[i], value, &set_flags);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS);

	err = options[i]->fn.get_value(options[i], &value);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(value.dbl, 1000.0); // check new value has been taken into account

	value.dbl = 50.0; // restore previous value
	err = options[i]->fn.set_value(options[i], value, &set_flags);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS);
}


static void tests_sane_set_resolution_ko(void)
{
	enum lis_error err;
	struct lis_option_descriptor **options = NULL;
	union lis_value value;
	int i;
	int set_flags;

	err = g_test_device->get_options(g_test_device, &options);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_NOT_EQUAL(options, NULL);

	for (i = 0 ; options[i] != NULL ; i++) {
		if (strcmp(options[i]->name, "resolution") == 0) {
			break;
		}
	}
	LIS_ASSERT_NOT_EQUAL(options[i], NULL);

	LIS_ASSERT_EQUAL(options[i]->value.type, LIS_TYPE_DOUBLE);
	LIS_ASSERT_EQUAL(options[i]->constraint.type, LIS_CONSTRAINT_RANGE);
	LIS_ASSERT_EQUAL(options[i]->constraint.possible.range.min.dbl, 1.0);
	LIS_ASSERT_EQUAL(options[i]->constraint.possible.range.max.dbl, 1200.0);
	LIS_ASSERT_EQUAL(options[i]->constraint.possible.range.interval.dbl, 1.0);

	value.dbl = 2000.0; // out of range
	err = options[i]->fn.set_value(options[i], value, &set_flags);
	// but for some reason, dum-dum says "ok" ...
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS | LIS_SET_FLAG_INEXACT);

	err = options[i]->fn.get_value(options[i], &value);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(value.dbl, 1200.0); // check new value has been taken into account

	value.dbl = 50.0; // restore previous value
	err = options[i]->fn.set_value(options[i], value, &set_flags);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS);
}


static void tests_sane_scan_parameters(void)
{
	enum lis_error err;
	struct lis_option_descriptor **options = NULL;
	union lis_value value;
	int resolution_idx = -1;
	int mode_idx = -1;
	int i;
	int set_flags;
	struct lis_scan_parameters scan_parameters;

	err = g_test_device->get_options(g_test_device, &options);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_NOT_EQUAL(options, NULL);

	for (i = 0 ; options[i] != NULL ; i++) {
		if (strcmp(options[i]->name, "mode") == 0) {
			mode_idx = i;
		} else if (strcmp(options[i]->name, "resolution") == 0) {
			resolution_idx = i;
		}
	}
	LIS_ASSERT_NOT_EQUAL(mode_idx, -1);
	LIS_ASSERT_NOT_EQUAL(resolution_idx, -1);

	value.string = "Color";
	err = options[mode_idx]->fn.set_value(options[mode_idx], value, &set_flags);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS | LIS_SET_FLAG_MUST_RELOAD_OPTIONS);

	err = g_test_device->get_options(g_test_device, &options);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_NOT_EQUAL(options, NULL);

	value.dbl = 100.0;
	err = options[resolution_idx]->fn.set_value(options[resolution_idx], value, &set_flags);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS);

	err = g_test_device->get_scan_parameters(g_test_device, &scan_parameters);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(scan_parameters.format, LIS_IMG_FORMAT_RAW_RGB_24);
	LIS_ASSERT_EQUAL(scan_parameters.width, 314);
	LIS_ASSERT_EQUAL(scan_parameters.height, 393);
	LIS_ASSERT_EQUAL(scan_parameters.image_size, 370206);

	value.dbl = 50.0;
	err = options[resolution_idx]->fn.set_value(options[resolution_idx], value, &set_flags);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS);

	err = g_test_device->get_scan_parameters(g_test_device, &scan_parameters);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(scan_parameters.format, LIS_IMG_FORMAT_RAW_RGB_24);
	LIS_ASSERT_EQUAL(scan_parameters.width, 157);
	LIS_ASSERT_EQUAL(scan_parameters.height, 196);
	LIS_ASSERT_EQUAL(scan_parameters.image_size, 92316);

	value.string = "Gray";
	err = options[mode_idx]->fn.set_value(options[mode_idx], value, &set_flags);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS | LIS_SET_FLAG_MUST_RELOAD_OPTIONS);

	err = g_test_device->get_options(g_test_device, &options);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_NOT_EQUAL(options, NULL);

	value.dbl = 100.0;
	err = options[resolution_idx]->fn.set_value(options[resolution_idx], value, &set_flags);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(set_flags, LIS_SET_FLAG_MUST_RELOAD_PARAMS);

	err = g_test_device->get_scan_parameters(g_test_device, &scan_parameters);
	LIS_ASSERT_TRUE(LIS_IS_OK(err));
	LIS_ASSERT_EQUAL(scan_parameters.format, LIS_IMG_FORMAT_GRAYSCALE_8);
	LIS_ASSERT_EQUAL(scan_parameters.width, 314);
	LIS_ASSERT_EQUAL(scan_parameters.height, 393);
	LIS_ASSERT_EQUAL(scan_parameters.image_size, 123402);
}


int register_tests(void)
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("Sane", tests_sane_init, tests_sane_clean);
	if (suite == NULL) {
		fprintf(stderr, "CU_add_suite() failed\n");
		return 0;
	}

	if (CU_add_test(suite, "list_devices()", tests_sane_list_devices) == NULL
			|| CU_add_test(suite, "get_device() ko", tests_sane_get_device_ko) == NULL
			|| CU_add_test(suite, "get_children()", tests_sane_item_get_children) == NULL
			|| CU_add_test(suite, "get resolution", tests_sane_get_resolution) == NULL
			|| CU_add_test(suite, "set_resolution", tests_sane_set_resolution) == NULL
			|| CU_add_test(suite, "set_resolution_ko",
				tests_sane_set_resolution_ko) == NULL
			|| CU_add_test(suite, "scan_parameters", tests_sane_scan_parameters) == NULL) {
		fprintf(stderr, "CU_add_test() has failed\n");
		return 0;
	}

	return 1;
}
