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

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/info.h"
#include "../util/mem.h"

// err module
#include "err.h"

// define a reasonable amount of errors
// that the circular linked list will hold
#define ERR_MAX 10

// error_t type for holding error information
typedef struct err_t {
	const char *msg;
	err_code_t code;
	int errno_state;
} err_t;

// node type of circular linked list
typedef struct err_node_t {
	err_t error;
	struct err_node_t *next;
} err_node_t;

typedef struct err_list_t {
	// the current error position
	err_node_t *tail;
	// head of the list to keep track of the first element that has been added
	// sooner or later this will be overwrite previous errors by newer errors
	err_node_t *head;
	// we will keep track of how many list elements we already are
	// because we don't need a gigantic list of errors and
	// if the list is full the new errors will replace older ones
	// and once we will reach the ERR_MAX value it will keep updating itself
	// overwriting elements this way when the list will unroll
	uint8_t n;
} err_list_t;

// state of the circular list
static err_list_t *err;

// err_list_new create new circular linked list
// also create the first node and assign info to it
static err_list_t *err_list_new(const char *m, err_code_t c, int s)
{
	if (err)
		INFOEE("[ERROR] Can't make a new list, please destroy the old one first");

	err_list_t *e = xzmalloc(sizeof(*e));
	e->tail		  = xzmalloc(sizeof(err_node_t));
	// assign the info to the current node
	e->tail->error.msg		   = m;
	e->tail->error.code		   = c;
	e->tail->error.errno_state = s;
	e->n					   = 1;
	// make head to point to the first node
	e->head = e->tail;
	// always point to the oldest node in the list
	e->tail->next = e->head;

	return e;
}

// err_node_new append a new node into to circular list
static void err_node_new(err_list_t *l, const char *m, err_code_t c, int s)
{
	if (!l)
		INFOEE("[ERROR] can't add new node to invalid NULL list");

	err_node_t *ptr = xmalloc(sizeof(*ptr));
	// add info
	ptr->error.msg		   = m;
	ptr->error.code		   = c;
	ptr->error.errno_state = s;
	ptr->next			   = l->head;
	// make next point to new ptr
	l->tail->next = ptr;
	// make the current
	l->tail = l->tail->next;
}

// err_write_node overwrite the next node to be the new error
static void err_write_node(err_list_t *l, const char *m, err_code_t c, int s)
{
	// get the next pointer
	l->tail = l->tail->next;
	// assign new info
	l->tail->error.msg		   = m;
	l->tail->error.errno_state = s;
	l->tail->error.code		   = c;
}

// err_new construct new error with message, code and errno
void err_new(const char *msg, err_code_t code, int save)
{
	// if the list is empty
	if (!err) {
		err = err_list_new(msg, code, save);
	} else if (err->n < ERR_MAX) {
		// if we have already a list but is not full
		err_node_new(err, msg, code, save);
		err->n++;
	} else {
		// list is full so we will overwith the oldest error
		err_write_node(err, msg, code, save);
	}
}

// err_last construct to get the last error that had been written
// the values will be saved to msg, code, save
// very important that msg needs to be null terminated
void err_last(char *msg, err_code_t *code, int *save)
{
	if (!err) {
		INFO("[WARNING] Can't get the last error because the list is empty");
		return;
	}

	if (!err->tail) {
		INFOEE("[ERROR] Please check the internal state of the list");
	}

	//TODO(hoenir): we must find a better way
	// to copy the content msg err into msg
	// it's not so safe with strcpy,
	// the error.msg must be '\0' terminated.
	strcpy(msg, err->tail->error.msg);
	*code = err->tail->error.code;
	*save = err->tail->error.errno_state;
}

// err_list_free free the circular list element by element
static bool err_list_free(err_list_t **l)
{
	if (!l) {
		return false;
	}

	uint8_t i = 0;
	for (i = 0; i < (*l)->n; i++) {
		(*l)->head = (*l)->tail->next;
		xfree((*l)->tail);
		(*l)->tail = (*l)->head;
	}

	(*l)->n = 0;
	xfree(*l);
	*l = NULL;
	return true;
}

// err_destroy wrapper around err_list_free()
void err_destroy(void)
{
	if (!err_list_free(&err))
		INFOEE("[ERROR] Can't free error list, list was not initilized");
}

// err_dump dump all the errors to stderr
// note that err_dump is no-op if the list is not valid
void err_dump(void)
{
	if (!err) {
		INFO("[WARNING] No list found for dumping");
		return;
	}

	uint8_t i	 = 0;
	err_node_t *p = err->tail->next;
	// traverse all the nodes
	for (i = 0; i < err->n; i++) {
		fprintf(stderr, "[ERROR] %s\n", p->error.msg);
		p = p->next;
	}
	// yeah I know I should free the node that was traversed but I'm
	// feeling lazy and I will use this func instead of cluttering the above code
	err_destroy();
}

// err_find find errors with the msg or code or errno
bool err_find(const char *msg, err_code_t code, int save)
{
	if (!err) {
		INFO("[WARNING] Cloud not find any node because there is no list");
		return false;
	}

	uint8_t i	 = 0;
	err_node_t *p = err->tail;

	for (i = 0; i < err->n; i++)
		if (!strcmp(p->error.msg, msg) || ((p->error.code == code) || (p->error.errno_state == save)))

			return true;

	return false;
}

// err_info print all the circular list members with their message
void err_info(void)
{
	if (!err) {
		INFO("[WARNING] No list found fod dumping");
		return;
	}

	uint8_t i	 = 0;
	err_node_t *p = err->tail->next;
	// traverse all the nodes
	for (i = 0; i < err->n; i++) {
		INFO(p->error.msg);
		p = p->next;
	}
}

// test if the internal list is empty
bool err_empty(void)
{
	if (!err)
		return true;

	return false;
}

// cmp compare two errors if their equal
static bool cmp(const err_node_t a, const err_node_t b)
{
	if ((!strcmp(a.error.msg, b.error.msg)) && (a.error.code == b.error.code) && (a.error.errno_state == b.error.errno_state))
		return true;

	return false;
}

// err_prev get the previous error infos inside msg, code, and save
void err_prev(char *msg, err_code_t *code, int *save)
{
	bool eq = false;
	if (!err) {
		INFO("[WARNING] Cloud not find the previous error because there is no list");
		return;
	}

	err_node_t *p = err->tail->next;
	err_node_t *s = NULL;

	while (true) {
		eq = cmp(*p, *(err->tail));
		if (eq) {
			strcpy(msg, s->error.msg);
			*code = s->error.code;
			*save = s->error.errno_state;
			return;
		}
		s = p;					  // save the prev state
		p = p->next;
	}
}

// err_prev_is test when ever the previous error match the code
bool err_prev_code_is(err_code_t code)
{
	bool eq = false;
	if (!err)
		INFOEE("[WARNING] Cloud not check the previous error because there is no list");

	err_node_t *p = err->tail->next;
	err_node_t *s = NULL;

	while (true) {
		// go and step through the list
		// and stop when we start
		eq = cmp(*p, *(err->tail));
		if (eq)
			break;
		s = p;
		p = p->next;
	}

	// we have a single error in the list
	if ((!s) && (err->tail->error.code == code))
		return true;

	if (s->error.code == code)
		return true;

	return false;
}

// err_prev_msg_is test when ever the previous error msg is equl to msg
bool err_prev_msg_is(const char *msg)
{
	bool eq = false;
	if (!err)
		INFOEE("[WARNING] Cloud not check the previous error because there is no list");

	err_node_t *p = err->tail->next;
	err_node_t *s = NULL;

	while (true) {
		// go and step through the list
		// and stop when we start
		eq = cmp(*p, *(err->tail));
		if (eq)
			break;
		s = p;
		p = p->next;
	}

	// we have a single error in the list
	if ((!s) && (!strcmp(err->tail->error.msg, msg)))
		return true;

	if (!strcmp(s->error.msg, msg))
		return true;

	return false;
}

// err_prev_save_is test when ever the previous save errno state is equl to save
bool err_prev_save_is(const int save)
{
	bool eq = false;
	if (!err)
		INFOEE("[WARNING] Cloud not check the previous error because there is no list");

	err_node_t *p = err->tail->next;
	err_node_t *s = NULL;

	while (true) {
		// go and step through the list
		// and stop when we start
		eq = cmp(*p, *(err->tail));
		if (eq)
			break;
		s = p;
		p = p->next;
	}

	// we have a single error in the list
	if ((!s) && (err->tail->error.errno_state == save))
		return true;

	if (s->error.errno_state == save)
		return true;

	return false;
}

// test if the new error is equal to code
bool err_this(err_code_t code)
{
	if (err)
		if (err->tail->error.code == code)
			return true;

	return false;
}
