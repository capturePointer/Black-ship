#include <errno.h>
#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include <lib/err.h>
#include <lib/mem.h>
#include <lib/util.h>

static void port_conv_test(void **state)
{
	(void)state;

	const char *p1 = "5213";
	const char *p2 = "-3213";
	const char *p3 = "70000";
	const char *p4 = "ufiusahfs";

	uint16_t r1 = port_conv(p1);
	assert_int_equal(r1, 5213);
	assert_true(err_empty());

	uint16_t r2 = port_conv(p2);
	assert_int_equal(r2, 0);
	assert_false(err_empty());
	assert_true(err_prev_code_is(ERRCONVPORT));

	uint16_t r3 = port_conv(p3);
	assert_int_equal(r3, 0);
	assert_false(err_empty());
	assert_true(err_prev_code_is(ERRCONVPORT));

	uint16_t r4 = port_conv(p4);
	assert_int_equal(r4, 0);
	assert_false(err_empty());
	assert_true(err_prev_code_is(ERRCONVPORT));

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
	char *p5 = strdup("0000-0000");

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
	assert_int_equal(r1, r2);
	assert_false(err_empty());
	assert_true(err_prev_code_is(ERRCONVPORT));

	// p5
	port_conv_range(p5, &r1, &r2);
	assert_int_equal(r1, 0);
	assert_int_equal(r2, 0);

	xfree(p1);
	xfree(p2);
	xfree(p3);
	xfree(p4);
	xfree(p5);

	err_destroy();
}

static void filter_number_test(void **state)
{
	(void)state;
	char *p1 = strdup("523");
	char *p2 = strdup("423sad");
	char *p3 = strdup("dusjiuf");

	assert_true(filter_number(p1));
	assert_false(filter_number(p2));
	assert_false(filter_number(p3));

	free(p1);
	free(p2);
	free(p3);
}

static void valid_ip_test(void **state)
{
	(void)state;
	bool f;
	char *p1 = strdup("192.168.23.232");
	assert_non_null(p1);

	char *p2 = strdup("FE80:0000:0000:0000:0202:B3FF:FE1E:8329");
	assert_non_null(p2);

	char *p3 = strdup("1923.3123.312.31");
	assert_non_null(p3);

	char *p4 = strdup("FFF:dsauhdif2143!:31@:#1@:#!:@3!:#12");
	assert_non_null(p4);

	f = valid_ip(p1);
	assert_true(f);
	f = valid_ip(p2);
	assert_true(f);
	f = valid_ip(p3);
	assert_false(f);
	f = valid_ip(p4);
	assert_false(f);

	xfree(p1);
	xfree(p2);
	xfree(p3);
	xfree(p4);
}

static void xsprintf_test(void **state)
{
	(void)state;
	int b = 10;
	char *buff = NULL;
	assert_null(buff);
	buff = xsprintf("Today this is my %d with %s",b,"Adams");
	assert_non_null(buff);
	size_t sz = strlen(buff);
	assert_true((sz > 0));
	xfree(buff);
}

static void port_random_test(void **state)
{
	(void)state;
	bool f = false;
	uint16_t n = 0;
	port_seeds();
	f = err_this(ERRENTROPY);
	assert_false(f);
	
	for(int i = 0; i<10000000; i++) {
		n = port_random();
		f = ((n >= 0) && (n < UINT16_MAX))? true:false;
		assert_true(f);
	}
}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(filter_number_test),
		cmocka_unit_test(port_conv_test),
		cmocka_unit_test(port_conv_range_test),
		cmocka_unit_test(valid_ip_test),
		cmocka_unit_test(xsprintf_test),
		cmocka_unit_test(port_random_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
