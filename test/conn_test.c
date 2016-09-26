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
	assert_non_null(conn->c4);
	assert_non_null(conn->c4->addr);
	assert_non_null(conn->c4->buff);
	assert_non_null(conn->c6);
	assert_non_null(conn->c6->addr);
	assert_non_null(conn->c6->buff);

	conn_free(conn);
	*(void**)&conn = NULL;
	assert_null(conn);
}

static void conn_addr_setup_test(void **state)
{
	(void)state;
	// create new ipv4 conn
	conn_t *conn = conn_new();
	assert_non_null(conn);
	assert_non_null(conn->c4);
	assert_non_null(conn->c4->addr);
	assert_non_null(conn->c4->buff);
	assert_non_null(conn->c6);
	assert_non_null(conn->c6->addr);
	assert_non_null(conn->c6->buff);

	conn_hints info4 = {
		.proto = "21",
		.host  = "127.0.0.1",
		.hints = {
			.ai_family   = AF_INET,
			.ai_socktype = SOCK_STREAM,
			.ai_flags	= 0 }
	};

	conn_addr4_setup(conn, info4);
	assert_true((conn->c4->addr->sin_port) > 0);
	assert_int_equal(conn->c4->addr->sin_port, 21);
	assert_int_equal(conn->c4->addr->sin_family, AF_INET);

	conn_free(conn);
	*(void**)&conn = NULL;
	assert_null(conn);

	conn = conn_new();
	assert_non_null(conn);
	assert_non_null(conn->c4);
	assert_non_null(conn->c4->addr);
	assert_non_null(conn->c4->buff);
	assert_non_null(conn->c6);
	assert_non_null(conn->c6->addr);
	assert_non_null(conn->c6->buff);

	conn_hints info6 = {
		.proto = "80",
		.host  = "::1",
		.hints = {
			.ai_family   = AF_INET6,
			.ai_socktype = SOCK_STREAM,
			.ai_flags	= 0 }
	};

	conn_addr6_setup(conn, info6);

	assert_true(((conn->c6->addr->sin6_port) > 0));
	assert_int_equal(conn->c6->addr->sin6_port, 80);
	assert_int_equal(conn->c6->addr->sin6_family, AF_INET6);

	conn_free(conn);
	*(void**)&conn = NULL;
	assert_null(conn);
}
int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(conn_new_test),
		cmocka_unit_test(conn_addr_setup_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
