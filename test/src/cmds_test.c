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

static void valid_attack_test(void **state)
{
	(void)state;
	char *p1 = strdup("udp");
	assert_non_null(p1);

	char *p2 = strdup("uhdusad");
	assert_non_null(p2);

	assert_true(valid_attack(p1));
	assert_false(valid_attack(p2));


	xfree(p1);
	xfree(p2);
}
int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(valid_attack_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
