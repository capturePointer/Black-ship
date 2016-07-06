#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include "../../lib/util/util.h"
#include "../../src/cmds.h"
#include "../../src/opts.h"

static void argp_parse_test(void **state)
{
	(void)state;
	char **argv = NULL;
	/* {"./blackship", "-h", "192.168.122.21", */
	/* "-p", "6421", "-a", "udp-flood", NULL}; */

	size_t len = sizeof(char *);
	argv	   = xzmalloc(8 * len);

	argv[0] = strdup("./blackship");
	argv[1] = strdup("-h");
	argv[2] = strdup("192.168.122.122");
	argv[3] = strdup("-p");
	argv[4] = strdup("4423");
	argv[5] = strdup("-a");
	argv[6] = strdup("udp-flood");
	argv[7] = NULL;

	for (uint8_t i = 0; i < 7; i++) {
		assert_non_null(argv[i]);
	}

	assert_null(argv[7]);

	arguments arg;
	memset(&arg, 0, sizeof(arguments));

	argp_parse(&argp, 7, argv, 0, 0, &arg);

	assert_string_equal(arg.host, "192.168.122.122");
	assert_string_equal(arg.attack, "udp-flood");
	assert_int_equal(arg.list_attacks, NO_LIST);
	assert_int_equal(arg.port.low, 0);
	assert_int_equal(arg.port.high, 0);
	assert_int_equal(arg.port.n, 4423);
	assert_int_equal(arg.port.random, 0);
	assert_int_equal(arg.host_type, IPV4);

	for (uint8_t i = 0; i < 7; i++) {
		xfree(argv[i]);
	}
	xfree(argv);
}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(argp_parse_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
