#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include "../../lib/err/err.h"

static int DEBUG;

#define MAX 13
// init variables
const char *msg[MAX] = {
	"Error reporting io",
	"This is another error",
	"This is likly my fault",
	"Error prone tcp conn",
	"Give me some god error logic",
	"Also the error is gone wild",
	"My error is error of the error",
	"CS CS ERROR CS",
	"Killing timout dos attack error",
	"LOREM LOREM ipsum error",
	"Please note that this is actually an error",
	"Please Work you with the params error",
	"Kill the errors from the program",
};
static err_code_t code[MAX] = {
	ERRUNKNOWN, ERRUNKNOWN, ERRTCPCONN,
	ERRUNKNOWN, ERRUNKNOWN, ERRUDPCONN,
	ERRUNKNOWN, ERRUNKNOWN, ERRHOSTUNREACHED,
	ERRUNKNOWN, ERRHOSTUNREACHED, ERRTCPCONN,
	ERRUNKNOWN,
};
static int errnos[MAX] = { EIO, EOF, ERANGE, EBUSY, EPIPE, ESRCH, ENOSPC,
						   ENOTSUP, ENOSPC, ENOMSG, ENOTTY, EIDRM, ETIME };

static void new_error_test(void **state)
{
	(void)state;

	// init variables
	const char *mms  = "Error give me test";
	err_code_t ccode = ERRTCPCONN;
	errno			 = ENOMEM;					  // 0xC -> 12
	// call the func

	assert_true(err_empty());
	err_new(mms, ccode, errno);

	// some testing vars
	char *k = calloc(strlen(mms) + 1, sizeof(char));
	err_code_t mkk;
	int mk = -1;
	//TODO
	// get the vars	that were sumbited
	err_last(k, &mkk, &mk);

	// check them
	assert_false(err_empty());
	assert_memory_equal(mms, k, strlen(mms) + 1);
	assert_string_equal(mms, k);
	assert_int_equal(ccode, mkk);
	assert_int_equal(errno, mk);

	// if everything is fine free the meme and print out
	free(k);
	err_destroy();
	assert_true(err_empty());
}

static void multi_error_dump_test(void **state)
{
	(void)state;
	assert_true(err_empty());
	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());

// if everything is fine free the meme and print out
if(DEBUG)
	err_dump();
else
	err_destroy();

	assert_true(err_empty());
}

static void multi_error_test(void **state)
{
	(void)state;
	assert_true(err_empty());
	// crate
	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());
	// if everything is fine free the meme and print out
	err_destroy();
	assert_true(err_empty());
}

static void stress_error_test(void **state)
{
	(void)state;
	// init variables
	const char *msg = "Error give me test";
	err_code_t code = ERRTCPCONN;
	errno			= ENOMEM;

	//call the func
	for (int i = 0; i < 10000; i++) {
		assert_true(err_empty());
		for (int j = 0; j < 10000; j++) {
			err_new(msg, code, errno);
		}
		err_destroy();
		assert_true(err_empty());
	}
}

static void find_error_test(void **state)
{
	(void)state;

	assert_true(err_empty());
	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());
	assert_true(err_find(msg[4], 0, -5));
	assert_true(err_find("", ERRUNKNOWN, 0));
	assert_false(err_find("", -100, -100));

// if everything is fine free the name and print out
if(DEBUG)
	err_dump();
else
	err_destroy();

	assert_true(err_empty());
}

static void prev_error_test(void **state)
{
	(void)state;
	assert_true(err_empty());
	for (int i = 0; i < 4; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());
	// some testing vars
	char *k = calloc(23, sizeof(char));
	err_code_t mkk;
	int mk = -1;

	err_prev(k, &mkk, &mk);
	assert_string_equal(k, msg[2]);
	assert_int_equal(mkk, code[2]);
	assert_int_equal(mk, errnos[2]);

	// if everything is fine free the name and print out
	err_destroy();
	free(k);
	assert_true(err_empty());
}

static void prev_error_is_test(void **state)
{
	(void)state;

	assert_true(err_empty());
	for (int i = 0; i < 4; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());

	assert_true(err_prev_code_is(ERRTCPCONN));
	assert_false(err_prev_code_is(ERRUNKNOWN));

	assert_true(err_prev_msg_is("This is likly my fault"));
	assert_false(err_prev_msg_is("Unkndusahduhsa"));

	assert_true(err_prev_save_is(ERANGE));
	assert_false(err_prev_save_is(EIO));

	assert_true(err_this(ERRUNKNOWN));
	assert_false(err_this(ERRTCPCONN));

	// if everything is fine free the name and print out
	err_destroy();
	assert_true(err_empty());
}

static void err_this_test(void **state)
{
	(void)state;

	assert_true(err_empty());

	assert_true(err_empty());
	for (int i = 0; i < 4; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}
	assert_false(err_empty());

	assert_true(err_this(code[3]));						// ERRUNKNOWN
	assert_false(err_this(code[2]));					// ERRTCPCON

	err_destroy();
	assert_true(err_empty());
}

int main(int argc, char **argv)
{
	// if we want to debug
	if (argc == 2) {
		if (!strncmp(argv[1], "-debug", 6)) {
			DEBUG = 1;
		}
	}

	cmocka_set_message_output(CM_OUTPUT_STDOUT);
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(new_error_test),
		cmocka_unit_test(multi_error_dump_test),
		cmocka_unit_test(multi_error_test),
		cmocka_unit_test(stress_error_test),
		cmocka_unit_test(find_error_test),
		cmocka_unit_test(prev_error_test),
		cmocka_unit_test(prev_error_is_test),
		cmocka_unit_test(err_this_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
