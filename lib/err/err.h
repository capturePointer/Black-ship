// Copyright [2016] [hoenir]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

// define all error codes that might exist
// also this way we can combine errors codes and check
// in a more reasonable fashion
typedef enum err_code {
	ERRUNKNOWN = 0, // unknown error
	ERRTCPCONN, // error tcp conn
	ERRATTACKUNSUPPORTED, // attack up unsuported
	ERRUDPCONN, // error udp conn
	ERRIPADDR, //error invalid ip address
	ERRTIMEOUT, // error timing out connection
	ERRHOSTUNREACHED, // error host unreached
	ERRCONVPORT, // error conversion port
	ERR_LAST, // NOT TO BE USED 
} err_code_t;

// write new errors in the list
// if the list is empty it will create and append a new node with info
// if the list is not empty and not full it will append a new node with info
// if the list is full it will overwrite the oldest error in the list
extern void err_new(const char *message, err_code_t code, int save);
// assign from the current pointer of the list of errors into params
extern void err_last(char *msg, err_code_t *code, int *save);
// destroy all nodes of the list and reset the list state to NULL
// it's recomanded to be used when exiting on critical functions and to shut down the program gracefully
extern void err_destroy(void);
// destroy all nodes of the list and reset the list state to NULL
// printing a nice error dump of all errors from the oldest to newest
// to be used on panic errors
extern void err_dump(void);
// search for the error and if it's found return true
extern bool err_find(const char *msg, err_code_t code, int save);
// test the list is valid
extern bool err_empty(void);
// the the previous error lenght-1 pos in the list
extern void err_prev(char *msg, err_code_t *code, int *save);
// dump all information without exiting
extern void err_info(void);
// test when ever the previous error is equal to code
extern bool err_prev_code_is(err_code_t code);
// test when ever the previous msg is equal to msg
extern bool err_prev_msg_is(const char *msg);
// test when ever the previous errno state is equal to save
extern bool err_prev_save_is(const int save);
// test if the new error is equl to code err->this.error
extern bool err_this(err_code_t code);

#endif /* ERROR_H*/
