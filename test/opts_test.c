#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include <lib/util.h>
#include <lib/mem.h>
#include <src/cmds.h>
#include <src/opts.h>

static void argp_opt_test(void **state)
{
	(void)state;
	arguments args = {
		.host = (char*)"127.0.0.1",
		.attack = UDP_FLOOD,
		.host_type = IPV4,
		.port = {
			.n = 5532,
			.low = 0,
			.high = 0,
			.random = false,
		},
		.packets = 5232,
		.list_attacks = NO_LIST
	};

	error_t err = parse_opt(RANDOM, (char*)"-r", (void*)&args);
	assert_int_equal(err, ARGP_KEY_ERROR);
}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(argp_opt_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
