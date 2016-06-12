#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include "../err.h"
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
err_code_t code[MAX] = {
	ERRUNKNOWN, ERRUNKNOWN, ERRTCPCONN,
	ERRUNKNOWN, ERRUNKNOWN, ERRUDPCONN,
	ERRUNKNOWN, ERRUNKNOWN, ERRHOSTUNREACHED,
	ERRUNKNOWN, ERRHOSTUNREACHED,  ERRTCPCONN,
	ERRUNKNOWN,
};
int errnos[MAX] = { EIO, EOF, ERANGE, EBUSY, EPIPE, ESRCH, ENOSPC,
					ENOTSUP, ENOSPC, ENOMSG, ENOTTY, EIDRM, ETIME };

void new_error_test()
{
	// init variables
	const char *mms  = "Error give me test";
	err_code_t ccode = ERRTCPCONN;
	errno			 = ENOMEM;					  // 0xC -> 12
	// call the func

	assert_true(err_empty());
	err_new(mms, ccode, errno);

	// some testing vars
	char *k = calloc(19, sizeof(char));
	err_code_t mkk;
	int mk = -1;

	// get the vars	that were sumbited
	err_last(k, &mkk, &mk);

	// check them
	assert_false(err_empty());
	assert_memory_equal(mms, k, 19);
	assert_string_equal(mms, k);
	assert_int_equal(ccode, mkk);
	assert_int_equal(errno, mk);

	// if everything is fine free the meme and print out
	free(k);
	err_destroy();
	assert_true(err_empty());

	printf("new_error_test() Passed\n");
}

void multi_error_dump_test()
{
	assert_true(err_empty());
	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());
	// if everything is fine free the meme and print out
	err_dump();
	assert_true(err_empty());
	printf("=======================>>> multi_error_dump_test() Passed\n");
}

void multi_error_test()
{
	assert_true(err_empty());
	// crate
	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());
	// if everything is fine free the meme and print out
	err_destroy();
	assert_true(err_empty());
	printf("=======================>>> multi_error_test() Passed\n");
}

void stress_error_test()
{
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

	printf("======================>>> stress_error_test() passed\n");
}

void find_error_test()
{
	assert_true(err_empty());
	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());
	assert_true(err_find(msg[4], 0, -5));
	assert_true(err_find("",ERRUNKNOWN, 0));
	assert_false(err_find("",-100,-100));

	// if everything is fine free the name and print out
	err_dump();
	assert_true(err_empty());

	printf("======================>>> find_error_test() Passed\n");
}
void prev_error_test()
{
	//TODO fix broken test
	assert_true(err_empty());
	for(int i = 0; i<4; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	assert_false(err_empty());
	// some testing vars
	char *k = calloc(19, sizeof(char));
	err_code_t mkk;
	int mk = -1;

	err_prev(k, &mkk, &mk);
	assert_string_equal(k, msg[2]);
	assert_int_equal(mkk,code[2]);
	assert_int_equal(mk, errnos[2]);
	
	// if everything is fine free the name and print out
	err_destroy();
	free(k);
	assert_true(err_empty());
	printf("======================>>> prev_error_test() Passed\n");
}

int main(void)
{
	new_error_test();
	multi_error_dump_test();
	multi_error_test();
	stress_error_test();
	find_error_test();

}
