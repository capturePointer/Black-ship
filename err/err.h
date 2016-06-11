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

// define all error codes that migh a app exist
// also this way we can combine errors codes and check
// in a more reasonable fashion
typedef enum err_code {
	ERRUNKNOWN = 0,
	ERRPROTOS,
	ERRTHAT,

	ERR_LAST, // NOT TO BE USED 
} err_code_t;

// write new errors in the list
// if the list is empty it will be created and append a new node with info
// if the list is not empty and not full it will append a new node with info
// if the list is full it will overwrite the oldest error in the list
extern void err_new(const char *message, err_code_t code, int save);
// print out the list of errors form the oldest to newest and exit out
extern void err_panic();
// assign from the current pointer of the list errors into params
extern void err_last(char *msg, err_code_t *code, int *save);
// destroy all nodes of the list and the list
// this it's advice to be used when exiting on critical functions and to
// shut down the program gracefully
extern void err_destroy();
extern void err_dump();
#endif /* ERROR_H*/
