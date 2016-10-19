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

typedef struct data_t {
	double a;
	int b;
	int c;
	uint64_t byte;
	char *mem_info;
} data_t;

static data_t *data_new()
{
	data_t *data = malloc(sizeof(*data));
	assert_non_null(data);
	memset(data, 0, sizeof(*data));
	data->a		   = 312.312;
	data->b		   = 1;
	data->c		   = 5;
	data->byte	 = 312315439802132;
	data->mem_info = strdup("This mem is is the mem");
	assert_non_null(data->mem_info);

	return data;
}

static void data_free(void *data)
{
	data_t *d = data;
	free(d->mem_info);
	free(d);
}

static list_t *new_list_test(void)
{
	list_t *link_list = list_new(sizeof(data_t), NULL, data_free);
	assert_null(link_list->head);
	assert_null(link_list->tail);
	assert_int_equal(link_list->n, 0);
	assert_int_equal(link_list->sz_blk, sizeof(data_t));
	assert_non_null(link_list->free);
	assert_null(link_list->cmp);

	return link_list;
}

static void free_list_test(list_t **l)
{
	assert_non_null(*l);
	list_free(l);
	assert_null(*l);
}

/////////////////////////////////////////////////////////// unit tests
static void list_init_test(void **state)
{
	(void)state;
	list_t *link_list = new_list_test();
	free_list_test(&link_list);
}

static void list_add_test(void **state)
{
	(void)state;
	list_t *link_list = new_list_test();
	data_t *blob	  = malloc(sizeof(*blob) * N);
	for (size_t i = 0; i < N; i++) {
		memset(blob, 0, sizeof(*blob));
		blob[i].a		 = 312.312;
		blob[i].b		 = 1;
		blob[i].c		 = 5;
		blob[i].byte	 = 312315439802132;
		blob[i].mem_info = strdup("This mem is is the mem");
		assert_non_null(blob[i].mem_info);

		list_add(link_list, &blob[i]);
	}
}
int main(void)
{
	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(list_init_test),
		cmocka_unit_test(list_add_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
