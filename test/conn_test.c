#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <cmocka.h>


#include <lib/mem.h>
#include <lib/conn.h>


static void conn_new_test(void **state)
{
	(void)state;
	conn_t *conn= conn_new(IPV4);
	
	assert_non_null(conn);
	xfree(conn->c4);
	xfree(conn);
}

int main(void) 
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(conn_new_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
