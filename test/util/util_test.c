#include <errno.h>
#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include "../../lib/err/err.h"
#include "../../lib/util/util.h"

static void port_conv_test(void **state)
{
	(void)state;

	const char *p1 = "5213";
	const char *p2 = "-3213";
	const char *p3 = "70000";
	uint16_t r1	= port_conv(p1);
	assert_int_equal(r1, 5213);
	assert_true(err_empty());

	uint16_t r2 = port_conv(p2);
	assert_int_equal(r2, 0);
	assert_false(err_empty());
	assert_true(err_prev_is(ERRCONVPORT));

	uint16_t r3 = port_conv(p3);
	assert_int_equal(r3, 0);
	assert_true(err_prev_is(ERRCONVPORT));
	assert_false(err_empty());

	err_destroy();
	//test for error
}
static void port_conv_range_test(void **state)
{
	(void)state;
	uint16_t r1 = 0, r2 = 0;

	char *p1 = strdup("100-200");
	char *p2 = strdup("5000-10000");
	char *p3 = strdup("231-100");
	char *p4 = strdup("1jhas-diajsudja");
	/* char *p5 = strdup("0000-0000"); */

	// p1
	port_conv_range(p1, &r1, &r2);
	assert_int_equal(r1, 100);
	assert_int_equal(r2, 200);
	assert_true(err_empty());

	// p2
	port_conv_range(p2, &r1, &r2);
	assert_int_equal(r1, 5000);
	assert_int_equal(r2, 10000);
	assert_true(err_empty());

	// p3
	port_conv_range(p3, &r1, &r2);
	assert_int_equal(r1, 100);
	assert_int_equal(r2, 231);
	assert_true(err_empty());

	// p4
	port_conv_range(p4, &r1, &r2);
	printf("%d\n", r1);
	printf("%d\n", r2);
	assert_false(err_empty());

	/* // p5 */
	/* port_conv_range(p5, &r1, &r2); */

	xfree(p1);
	xfree(p2);
	xfree(p3);
	xfree(p4);
	/* xfree(p5); */
	err_destroy();
}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(port_conv_test),
		cmocka_unit_test(port_conv_range_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
