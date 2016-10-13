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
	int n;
	size_t pik;
	float p;
}test_data;

static struct test_data *new_data(void)
{
	struct test_data *block = malloc(sizeof(block) *N);
	assert_non_null(block);
	for(size_t i=0; i<N; i++) {
		block[i].p = 312.5;
		block[i].n = 121;
		block[i].pik = 12;
	}
	return block;
}

static void list_init_test(void **state)
{
	(void)state;
	list_t *link_list = new_list_test();
	free_list_test(&link_list);
}

//TODO
/* static void list_hold_test(void **state) */
/* { */
/* 	(void)state; */
/* 	list_t *l = new_list_test(); */
/* 	struct test_data *d= new_data(); */
/* 	node_t *p = l->head; */
/* 	size_t i = 0; */
/*  */
/* 	free_list_test(&l); */
/* } */

int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(list_init_test),
	//	cmocka_unit_test(list_hold_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
