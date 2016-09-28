#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include <lib/mem.h>
#include <src/cmds.h>
#include <src/opts.h>

static void argp_parse_test1(void **state)
{
	unsigned long N_ARGS = 7;
	(void)state;
	char **argv = NULL;
	size_t sz   = sizeof(char *);
	argv		= xzmalloc((N_ARGS + 1) * sz);

	argv[0] = strdup("./blackship_test");
	argv[1] = strdup("-h");
	argv[2] = strdup("192.168.122.122");
	argv[3] = strdup("-p");
	argv[4] = strdup("4423");
	argv[5] = strdup("-a");
	argv[6] = strdup("udp");
	argv[7] = NULL;

	for (uint8_t i = 0; i < N_ARGS; i++) {
		assert_non_null(argv[i]);
	}

	assert_null(argv[N_ARGS]);

	arguments arg;
	memset(&arg, 0, sizeof(arguments));

	argp_parse(&argp, (int)N_ARGS, argv, 0, 0, &arg);

	assert_string_equal(arg.host, "192.168.122.122");
	assert_int_equal(arg.attack, UDP_FLOOD);
	assert_int_equal(arg.list_attacks, NO_LIST);
	assert_int_equal(arg.port.low, 0);
	assert_int_equal(arg.port.high, 0);
	assert_int_equal(arg.port.n, 4423);
	assert_int_equal(arg.port.random, 0);
	assert_int_equal(arg.host_type, IPV4);

	for (uint8_t i = 0; i < N_ARGS; i++) {
		xfree(argv[i]);
	}
	xfree(argv);
}

static void argp_parse_test2(void **state)
{
	unsigned long N_ARGS = 8;
	(void)state;
	char **argv = NULL;
	size_t sz   = sizeof(char *);
	argv		= xzmalloc((N_ARGS + 1) * sz);

	argv[0] = strdup("./blackship_test");
	argv[1] = strdup("-h");
	argv[2] = strdup("192.168.122.122");
	argv[3] = strdup("-p");
	argv[4] = strdup("4423");
	argv[5] = strdup("-a");
	argv[6] = strdup("udp");
	argv[7] = strdup("--i6");
	argv[8] = NULL;

	for (uint8_t i = 0; i < N_ARGS; i++) {
		assert_non_null(argv[i]);
	}

	assert_null(argv[N_ARGS]);

	arguments arg;
	memset(&arg, 0, sizeof(arguments));

	argp_parse(&argp, (int)N_ARGS, argv, 0, 0, &arg);

	assert_string_equal(arg.host, "192.168.122.122");
	assert_int_equal(arg.attack, UDP_FLOOD);
	assert_int_equal(arg.list_attacks, NO_LIST);
	assert_int_equal(arg.port.low, 0);
	assert_int_equal(arg.port.high, 0);
	assert_int_equal(arg.port.n, 4423);
	assert_int_equal(arg.port.random, 0);
	assert_int_equal(arg.host_type, IPV6); // this time test if ipv6 is set

	for (uint8_t i = 0; i < N_ARGS; i++) {
		xfree(argv[i]);
	}

	xfree(argv);
}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(argp_parse_test1),
		cmocka_unit_test(argp_parse_test2),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
