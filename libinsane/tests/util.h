#ifndef __LIBINSANE_TESTS_UTIL_H
#define __LIBINSANE_TESTS_UTIL_H

/* macros to satisfy static analysis when using Cunit:
 * Static analysis doesn't seem to get that CU_ASSERT_xxx() do stop the test
 * when they fails.
 */

#define LIS_ASSERT_TRUE(x) do { \
		CU_ASSERT_TRUE(x); \
		if (!(x)) return; \
	} while (0)

#define LIS_ASSERT_FALSE(x) do { \
		CU_ASSERT_FALSE(x); \
		if (x) return; \
	} while (0)

#define LIS_ASSERT_EQUAL(x, y) do { \
		CU_ASSERT_EQUAL(x, y); \
		if ((x) != (y)) return; \
	} while (0)

#define LIS_ASSERT_NOT_EQUAL(x, y) do { \
		CU_ASSERT_NOT_EQUAL(x, y); \
		if ((x) == (y)) return; \
	} while (0)

#endif
