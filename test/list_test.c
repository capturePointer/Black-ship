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

#include <lib/list.h>

#define N 10
static list_t *new_list_test(void)
{
	list_t *link_list = list_new(NULL,NULL);
	assert_null(link_list->head);
	assert_null(link_list->tail);
	assert_int_equal(link_list->n, 0);
	assert_int_equal(link_list->sz_blk, 0);
	assert_null(link_list->free);
	assert_null(link_list->cmp);

	return link_list;
}

static void free_list_test(list_t **l)
{
	assert_non_null(*l);
	list_free(l);
	assert_null(*l);
}

static void list_init_test(void **state)
{
	(void)state;
	list_t *link_list = new_list_test();
	free_list_test(&link_list);
}

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(list_init_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
