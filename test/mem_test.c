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

#define LEN 100

static void mem_malloc_test(void **state)
{
	(void)state;
	uint8_t *test = mem_malloc(LEN * sizeof(uint8_t));
	assert_non_null(test);
	mem_free(test);
	assert_non_null(test);
}

static void mem_calloc_test(void **state)
{
	(void)state;
	uint8_t *test = mem_calloc(LEN, sizeof(uint8_t));
	assert_non_null(test);
	mem_free(test);
	assert_non_null(test);
}

static void mem_zmalloc_test(void **state)
{
	(void)state;
	uint8_t *test = mem_zmalloc(LEN * sizeof(uint8_t));
	assert_non_null(test);
	mem_free(test);
	assert_non_null(test);
}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(mem_malloc_test),
		cmocka_unit_test(mem_calloc_test),
		cmocka_unit_test(mem_zmalloc_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
