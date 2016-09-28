#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include <lib/conn.h>
#include <lib/mem.h>

static void conn_new_test(void **state)
{
	(void)state;
	// create new ipv4 conn
	conn_t *conn = conn_new();
	assert_non_null(conn);
	assert_non_null(conn->addr);
	assert_non_null(conn->buff);
	conn_free(conn);
	*(void**)&conn = NULL;
	assert_null(conn);
}


int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(conn_new_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
