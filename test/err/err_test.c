#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include <lib/err.h>

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
	bool f = false;
	// init variables
	const char *mms  = "Error give me test";
	err_code_t ccode = ERRTCPCONN;
	errno			 = ENOMEM;					  // 0xC -> 12
	
	// call the func
	f = err_empty();
	assert_true(f);
	err_new(mms, ccode, errno);

	// some testing vars
	char *k = calloc(strlen(mms) + 1, sizeof(char));
	err_code_t mkk;
	int mk = -1;
	
	//TODO
	// get the vars	that were sumbited
	err_last(k, &mkk, &mk);

	// check them
	f = err_empty();
	assert_false(f);
	assert_memory_equal(mms, k, strlen(mms) + 1);
	assert_string_equal(mms, k);
	assert_int_equal(ccode, mkk);
	assert_int_equal(errno, mk);

	// if everything is fine free the meme and print out
	free(k);
	err_destroy();
	f = err_empty();
	assert_true(f);
}

static void multi_error_dump_test(void **state)
{
	(void)state;
	bool f = false;
	f = err_empty();
	assert_true(f);

	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	f = err_empty();
	assert_false(f);

// if everything is fine free the meme and print out
if(DEBUG)
	err_dump();
else
	err_destroy();

	f = err_empty();
	assert_true(f);
}

static void multi_error_test(void **state)
{
	(void)state;
	bool f = false;

	f = err_empty();
	assert_true(f);
	// crate
	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	f = err_empty();
	assert_false(f);
	// if everything is fine free the meme and print out
	err_destroy();

	f = err_empty();
	assert_true(f);
}

static void stress_error_test(void **state)
{
	(void)state;
	bool f = false;
	
	// init variables
	const char *msg = "Error give me test";
	err_code_t code = ERRTCPCONN;
	errno			= ENOMEM;

	//call the func
	for (int i = 0; i < 10000; i++) {
		f = err_empty();
		assert_true(f);
		for (int j = 0; j < 10000; j++) {
			err_new(msg, code, errno);
		}
		err_destroy();
		f = err_empty();
		assert_true(f);
	}
}

static void find_error_test(void **state)
{
	(void)state;
	bool f = false;
	assert_false(f);

	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	f = err_empty();
	assert_false(f);

	assert_true(err_find(msg[4], 0, -5));
	assert_true(err_find("", ERRUNKNOWN, 0));
	assert_false(err_find("", -100, -100));

// if everything is fine free the name and print out
if(DEBUG)
	err_dump();
else
	err_destroy();

	f = err_empty();
	assert_true(f);
}

static void prev_error_test(void **state)
{
	(void)state;
	bool f = false;
	f = err_empty();
	assert_true(f);
	for (int i = 0; i < 4; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	f = err_empty();
	assert_false(f);

	// some testing vars
	char *k = calloc(ERR_MSG_MAX, sizeof(char));
	assert_true(k != NULL);

	err_code_t mkk = 0;
	int mk = -1;

	err_prev(k, &mkk, &mk);
	assert_string_equal(k, msg[2]);
	assert_int_equal(mkk, code[2]);
	assert_int_equal(mk, errnos[2]);

	// if everything is fine free the name and print out
	err_destroy();
	free(k);

	f = err_empty();
	assert_true(f);
}

static void prev_error_test_depth(void **state) 
{
	(void)state;
	bool f = false;
	f = err_empty();
	assert_true(f);
	const char *trymsg = "This is another error from another place";
	for (int i = 0; i < 4; i++) {
		if (i == 2)
			err_new(trymsg, code[5], errnos[5]);
		else
			err_new(msg[i], code[i], errnos[i]);
	}

	f = err_empty();
	assert_false(f);
	
	// some testing vars
	char *k = calloc(ERR_MSG_MAX, sizeof(char));
	assert_true(k != NULL);

	err_code_t mkk = 0;
	int mk = -1;


	err_prev(k, &mkk, &mk);
	assert_string_equal(k, trymsg);
	assert_int_equal(mkk, code[5]);
	assert_int_equal(mk, errnos[5]);

	// if everything is fine free the name and print out
	err_destroy();
	free(k);

	f = err_empty();
	assert_true(f);
}
static void prev_error_test_max_msg(void **state)
{
	(void)state;
	bool f = false;

	// some testing vars
	char *k = calloc(ERR_MSG_MAX, sizeof(char));
	assert_true(k != NULL);
	// write in the k buffer ERR_MSG_MAX chars
	// without including the '\0'
	for (int i=0; i<ERR_MSG_MAX;i++){
		k[i] = 'A';
	}

	err_code_t mkk = 0;
	int mk = -1;

	// create the list
	f = err_empty();
	assert_true(f);
	for (int i = 0; i < 4; i++) {
		if (i == 2) 
			err_new(k, code[i], errnos[i]);
		else 
			err_new(msg[i], code[i], errnos[i]);
	}
	f = err_empty();
	assert_false(f);

	// test now the vals
	err_prev(k, &mkk, &mk);
	assert_true(k[ERR_MSG_MAX] == '\0');
	assert_int_equal(mkk, code[2]);
	assert_int_equal(mk, errnos[2]);

	// if everything is fine free the name and print out
	err_destroy();
	free(k);

	f = err_empty();
	assert_true(f);
}
static void prev_error_is_test(void **state)
{
	(void)state;
	bool f = false;

	f = err_empty();
	assert_true(f);

	for (int i = 0; i < 4; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}


	f = err_empty();
	assert_false(f);

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

	f = err_empty();
	assert_true(f);
}

static void err_this_test(void **state)
{
	(void)state;
	bool f = false;
	f = err_empty();
	assert_true(f);

	for (int i = 0; i < 4; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	f = err_empty();
	assert_false(f);

	assert_true(err_this(code[3]));						// ERRUNKNOWN
	assert_false(err_this(code[2]));					// ERRTCPCON

	err_destroy();

	f = err_empty();
	assert_true(f);
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
		cmocka_unit_test(prev_error_test_depth),
		cmocka_unit_test(prev_error_is_test),
		cmocka_unit_test(err_this_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
