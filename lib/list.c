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

#include <strings.h>

#include <lib/info.h>
#include <lib/mem.h>

#include "list.h"
#include <string.h>

/*
 * list_new(uint64_t n)
 *
 * @cmp - ptr to compare function
 * @free - ptr to free function
 * @size - size of the data that the node will hold
 *
 * Allocs a new empty linked list
 */
list_t *list_new(uint64_t size, cmp_fn_t cmp, free_fn_t free)
{
	list_t *list = xzmalloc(sizeof(*list));
	list->cmp = cmp;
	list->free = free;
	list->sz_blk = size;
	return list;
}

/*
 * list_add(list_t *l, void *blk)
 *
 * @l - ptr to linked list
 * @blk - block of mem, the data in the node
 * 
 * Add @blk to the next node in the linked list @l
 */
void list_add(list_t *l, void *blk)
{
	if ((!l->head) && (!l->tail)) {
		l->head = xmalloc(sizeof(node_t));
		l->head->blk = blk;
		l->tail = l->head;
		l->n = 1;
		return;
	}

	l->tail->next = xmalloc(sizeof(node_t));
	l->tail->next->blk = blk;
	l->tail = l->tail->next;
	l->n++;
}

/*
 * list_free
 *
 * @l - ptr to ptr to a linked list
 *
 * Free's memory from every node in the linked list.
 * After the free @l will ne NULL
 *
 */
void list_free(list_t **l)
{
	if (!*l)
		INFOEE("Could not release NULL list");
	if (!(*l)->free)
		INFOEE("Could not use NULL ptr free fn");
	if ((*l)->head == NULL)
		goto ret;

	node_t *head = (*l)->head;
	// if we have just one node.
	if (!head->next) {
		if (head->blk)
			(*l)->free(head->blk);
		xfree(head);
		goto ret;
	}

	// if we have more than just one node.
	node_t *next = NULL;
	while((*l)->n) {
		next = head->next;
		if(head->blk)
			(*l)->free(head->blk);
		xfree(head);
		head = next;
		(*l)->n--;
	}

ret:
	xfree(*l);
	*l = NULL;
}

