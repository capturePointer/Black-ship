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
	conn_buff_new(conn, UINT16_MAX);
	assert_non_null(conn);
	assert_non_null(conn->addr);
	assert_non_null(conn->buff);
	conn_buff_free(conn);
	conn_free(conn);
	*(void**)&conn = NULL;
	assert_null(conn);
}

static void conn_addr_setup_test_ivp4(void **state)
{
	(void)state;
	conn_t *conn = conn_new();
	conn_buff_new(conn, UINT16_MAX);
	assert_non_null(conn);
	assert_non_null(conn->addr);
	assert_non_null(conn->buff);

	conn_hints info4 = {
		.proto = "http",
		.host  = "127.0.0.1",
		.hints = {
			.ai_family   = AF_INET,
			.ai_socktype = SOCK_STREAM,
			.ai_flags	= 0
		}
	};

	conn_addr_setup(conn, info4);
	struct sockaddr_in *in;
	in = (struct sockaddr_in*)conn->addr;

	assert_true((in->sin_port > 0));
	assert_int_equal(in->sin_family, AF_INET);
	char buff[INET_ADDRSTRLEN];
	const char *err = inet_ntop(in->sin_family, &(in->sin_addr), buff, INET_ADDRSTRLEN);
	assert_non_null(err);
	assert_string_equal(buff, "127.0.0.1");

	conn_buff_free(conn);
	conn_free(conn);
	*(void**)&conn = NULL;
	assert_null(conn);
}

static void conn_addr_setup_test_ipv6(void **state)
{
	(void)state;
	conn_t *conn = conn_new();
	conn_buff_new(conn, UINT16_MAX);
	assert_non_null(conn);
	assert_non_null(conn->addr);
	assert_non_null(conn->buff);

	conn_hints info6 = {
		.proto = "ssh",
		.host  = "::1",
		.hints = {
			.ai_family   = AF_INET6,
			.ai_socktype = SOCK_STREAM,
			.ai_flags	= 0
		}
	};

	conn_addr_setup(conn, info6);
	struct sockaddr_in6 *in;
	in = (struct sockaddr_in6*)conn->addr;

	assert_true((in->sin6_port > 0));
	assert_int_equal(in->sin6_family, AF_INET6);
	char buff[INET6_ADDRSTRLEN];
	const char *err = inet_ntop(in->sin6_family, &(in->sin6_addr), buff, INET6_ADDRSTRLEN);
	assert_non_null(err);
	assert_string_equal(buff, "::1");

	conn_buff_free(conn);
	conn_free(conn);
	*(void**)&conn = NULL;
	assert_null(conn);

}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(conn_new_test),
		cmocka_unit_test(conn_addr_setup_test_ivp4),
		cmocka_unit_test(conn_addr_setup_test_ipv6),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
