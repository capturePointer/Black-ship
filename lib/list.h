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
//

#include <stdbool.h>
#include <stdint.h>


typedef bool (*cmp_fn_t)(void *, void *, uint64_t z);
typedef void (*free_fn_t)(void *);

typedef struct node_t {
	void *blk;
	struct node_t *next;
} node_t;

typedef struct list_t {
	// head of the linked list
	// the first element of the list
	node_t *head;
	// the last element of the list
	node_t *tail;
	// number of nodes
	uint64_t n;
	// size of the data
	uint64_t sz_blk; // size of the blk inside node
	// compare fn used for many operation on the linked list
	cmp_fn_t cmp;
	// free fn used for freeing the internal data in the node.
	free_fn_t free;
} list_t;

// alloc a new empty linked list setuping cmp and free methods
extern list_t *list_new(uint64_t size, cmp_fn_t cmp, free_fn_t free);
// free the linked list
extern void list_free(list_t **list);
extern void list_add(list_t *list, void *blk);
