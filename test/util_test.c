#include <errno.h>
#include <lib/mem.h>
#include <lib/util.h>
#include <limits.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cmocka.h>

static void port_conv_test(void **state)
{
	(void)state;

	int16_t p	  = 0;
	const char *p1 = "5213";
	const char *p2 = "-3213";
	const char *p3 = "70000";
	const char *p4 = "ufiusahfs";

	p = port_conv(p1);
	assert_int_equal(p, 5213);

	p = port_conv(p2);
	assert_int_equal(p, -1);

	p = port_conv(p3);
	assert_int_equal(p, -1);

	p = port_conv(p4);
	assert_int_equal(p, -1);
}

static void port_conv_range_test(void **state)
{
	(void)state;
	uint16_t r1 = 0, r2 = 0;
	int8_t err;
	char *p1 = strdup("100-200");
	char *p2 = strdup("5000-10000");
	char *p3 = strdup("231-100");
	char *p4 = strdup("1jhas-diajsudja");
	char *p5 = strdup("0000-0000");

	err = port_conv_range(p1, &r1, &r2);
	assert_int_equal(err, 0);
	assert_int_equal(r1, 100);
	assert_int_equal(r2, 200);

	err = port_conv_range(p2, &r1, &r2);
	assert_int_equal(err, 0);
	assert_int_equal(r1, 5000);
	assert_int_equal(r2, 10000);

	err = port_conv_range(p3, &r1, &r2);
	assert_int_equal(err, 0);
	assert_int_equal(r1, 100);
	assert_int_equal(r2, 231);

	err = port_conv_range(p4, &r1, &r2);
	assert_int_equal(err, -1);

	err = port_conv_range(p5, &r1, &r2);
	assert_int_equal(err, 0);
	assert_int_equal(r1, 0);
	assert_int_equal(r2, 0);
	assert_int_equal(r1, r2);

	xfree(p1);
	xfree(p2);
	xfree(p3);
	xfree(p4);
	xfree(p5);
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

static void port_random_test(void **state)
{
	(void)state;
	bool f	 = false;
	uint16_t n = 0;
	port_seeds();

	for (int i = 0; i < 10000000; i++) {
		n = port_random();
		f = ((n >= 0) && (n < UINT16_MAX)) ? true : false;
		assert_true(f);
	}
}

static void strconv_test(void **state)
{
	(void)state;

	char max_test[]		 = "18446744073709551615";
	char max_test_fail[] = "18446744073709551321321615ULL";
	char normal_test[]   = "12398175918273";
	char hex_test[]		 = "0x521Af";
	char string_test[]   = "dsuhfiudsahgiudshaiahfiudshgfsadf";
	char zero_test[]	 = "0";

	int64_t u = strconv(max_test, 10);
	assert_int_equal(u, ULLONG_MAX);

	u = strconv(zero_test, 10);
	assert_true(u != -1);
	assert_int_equal(u, 0);

	u = strconv(hex_test, 16);
	assert_true(u != -1);
	assert_int_equal(u, 0x521AF);

	u = strconv(string_test, 10);
	assert_int_equal(u, -1);

	u = strconv(normal_test, 10);
	assert_true(u != -1);
	assert_int_equal(u, 12398175918273);

	u = strconv(max_test_fail, 10);
	assert_int_equal(u, -1);
}

static bool sig_test_flag;

static void signal_test_cb(int sig)
{
	(void)sig;
	sig_test_flag = true;
}

static void signal_test(void **state)
{
	(void)state;
	pid_t pid = getpid();
	assert_false(sig_test_flag);

	// handle SIGALRM
	assert_int_equal(treat_signal(SIGALRM, signal_test_cb), 0);

	alarm(1);
	sleep(1);
	assert_true(sig_test_flag);

	sig_test_flag = false;					  // restart

	// handle SIGPIPE
	treat_signal(SIGPIPE, signal_test_cb);
	kill(pid, SIGPIPE);
	assert_true(sig_test_flag);

	sig_test_flag = false;					  // restart
}

static void array_size_macro_test(void **state)
{
	(void)state;

	const char *arr[] = {
		"a",
		"b",
		"c",
		"k",
		"l",
		"m",
	};

	const int narr[] = { 1, 2, 5, 42, 31, 26, 21, 6, 12, 6, 23, 0, 312 };

	assert_int_equal(ARRAY_SIZE(arr), 6);
	assert_int_equal(ARRAY_SIZE(narr), 13);

}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(filter_number_test),
		cmocka_unit_test(port_conv_test),
		cmocka_unit_test(port_conv_range_test),
		cmocka_unit_test(valid_ip_test),
		cmocka_unit_test(port_random_test),
		cmocka_unit_test(strconv_test),
		cmocka_unit_test(signal_test),
		cmocka_unit_test(array_size_macro_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
