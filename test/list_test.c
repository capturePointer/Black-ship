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
	list_t *link_list = list_new(N);
	assert_int_equal(link_list->n, N);
	assert_non_null(link_list->head);
	assert_non_null(link_list->tail);
	assert_null(link_list->tail->next);
	return link_list;
}

static void free_list_test(list_t **l)
{
	assert_non_null(*l);
	list_free(l);
	assert_null(*l);
}

struct test_data{
	int n; // 4
	size_t pik; //4
	// padding 8
	double p; // 8
}test_data;

static struct test_data *new_data(void)
{
	struct test_data *block = malloc(sizeof(*block)*N);
	assert_non_null(block);
	size_t i = 0;
	for (; i<N; i++) {
		block[i].n = 523;
		block[i].pik = 0;
		block[i].p = 312.312;
	}

	return block;
}

static void list_init_test(void **state)
{
	(void)state;
	list_t *link_list = new_list_test();
	free_list_test(&link_list);
}

static void freex_blk(void *blk)
{
	assert_non_null(blk);
	free(blk);
}
/*  */
/* static void list_hold_test(void **state) */
/* { */
/* 	(void)state; */
/* 	// alloc list */
/* 	list_t *l = new_list_test(); */
/* 	// alloc data */
/* 	struct test_data *d = new_data(); */
/* 	freeblk_init(freex_blk); */
/* 	for(size_t i = 0; i<N; i++) { */
/* 		list_add(l,	&d[i], sizeof(d[i])); */
/* 	} */
/* 	free_list_test(&l); */
/* } */

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(list_init_test),
		//cmocka_unit_test(list_hold_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
