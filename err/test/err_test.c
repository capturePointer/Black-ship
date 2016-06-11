#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include "../err.h"

void new_error_test()
{
	// init variables
	const char *msg = "Error give me test";
	err_code_t code = ERRPROTOS;
	errno			= ENOMEM;					 // 0xC -> 12
	// call the func

	err_new(msg, code, errno);

	// some testing vars
	char *k = calloc(19, sizeof(char));
	err_code_t mkk;
	int mk = -1;

	// get the vars	that were sumbited
	err_last(k, &mkk, &mk);

	// check them
	assert_memory_equal(msg, k, 19);
	assert_string_equal(msg, k);
	assert_int_equal(code, mkk);
	assert_int_equal(errno, mk);

	// if everything is fine free the meme and print out
	free(k);
	err_destroy();

	printf("new_error_test() Passed\n");
	
}

void multi_error_test()
{
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

/*
   THE ECHO SHOULD LOOK LIKE THIS
	FROM THE OLDEST TO NEWEST

[ERROR] Error prone tcp conn
[ERROR] Give me some god error logic
[ERROR] Also the error is gone wild
[ERROR] My error is error of the error
[ERROR] CS CS ERROR CS
[ERROR] Killing timout dos attack error
[ERROR] LOREM LOREM ipsum error
[ERROR] Please note that this is actually an error
[ERROR] Please Work you with the params error
[ERROR] Kill the errors from the program
*/
	err_code_t code[MAX] = {
		ERRUNKNOWN, ERRTHAT, ERRPROTOS,
		ERRUNKNOWN, ERRTHAT, ERRPROTOS,
		ERRUNKNOWN, ERRTHAT, ERRPROTOS,
		ERRUNKNOWN, ERRTHAT, ERRPROTOS,
		ERRUNKNOWN,
	};

	int errnos[MAX] = {
		EIO,
		EOF,
		ERANGE,
		EBUSY,
		EPIPE,
		ESRCH,
		ENOSPC,
		ENOTSUP,
		ENOSPC,
		ENOMSG,
		ENOTTY,
		EIDRM,
		ETIME,
	};

	// crate
	for (int i = 0; i < MAX; i++) {
		err_new(msg[i], code[i], errnos[i]);
	}

	// if everything is fine free the meme and print out
	err_dump();
	printf("multi_error_test() Passed\n");
}

int main(void)
{
	new_error_test();
	multi_error_test();
}
