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
#include "../util/util.h"
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
	// sooner or later this will be overwritten by newer errors
	err_node_t *head;
	// we will keep track of how many list elements we already have
	// because we don't need a giagacting list of errors and
	// if the list is full the newer errors will replace the older ones
	// and once we will reach the ERR_MAX value it will keep updating itself
	// overwriting the previously elements this way when the stack will unroll
	// we can debug more easly the program for errors in the case of critical
	// error we will echo the entire list.
	uint8_t n;
} err_list_t;

// state of the circular list
err_list_t *err = NULL;

// err_list_t * err_list_new
// create new circular linked list also create the first node
// and assign info to it
static err_list_t *err_list_new(const char *m, err_code_t c, int s)
{
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

// void err_node_new
// append a new node into to circular list
static void err_node_new(err_list_t *l, const char *m, err_code_t c, int s)
{
	err_node_t *ptr = xmalloc(sizeof(*ptr));
	// add info
	ptr->error.msg		   = m;
	ptr->error.code		   = c;
	ptr->error.errno_state = s;
	ptr->next = l->head;
	
	// make next point to new ptr
	l->tail->next = ptr;
	// make the current
	l->tail = l->tail->next;
}

// void err_write_node
// overwrite the next node to be a new error
// because the list is circular we will overwrite the oldest error
static void err_write_node(err_list_t *l, const char *m, err_code_t c, int s)
{
	// get the next pointer
	l->tail = l->tail->next;
	// assign new info
	l->tail->error.msg		   = m;
	l->tail->error.errno_state = s;
	l->tail->error.code		   = c;
}

// void err_new
// construct new error with a message, code and errno
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

// void err_last
// construct to gen the last error that was been writed
// the values will be saved to msg, code, save
// very important that msg to malloced and filled with 0
void err_last(char *msg, err_code_t *code, int *save)
{
	if (err) {
		if (err->tail) {
			strcpy(msg, err->tail->error.msg);
			*code = err->tail->error.code;
			*save = err->tail->error.errno_state;
		}
	}
}

void err_panic()
{
	uint8_t i = 0;
	for (i = 0; i < err->n; i++) {
		fprintf(stderr, "[ERROR] %s %s", err->tail->error.msg,
				strerror(err->tail->error.errno_state));
		err->tail = err->tail->next;
	}

	INFOEE("[STACK-TRACE] Panic because of errors.");
}

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

void err_destroy()
{
	if (!err_list_free(&err)) {
		INFOEE("[ERROR] Can't free error list, list was not initilized");
	}
}

void err_dump() {
	uint8_t i=0;
	err_node_t *p = err->tail->next;
	for(i=0; i<err->n; i++) {
		printf("[ERROR] %s\n", p->error.msg);
		p = p->next;
	}
	err_destroy();
}