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

/**
 * compare function used for many operation on the linked list
 * this will be writed with cmp_init.
 */
static compare_cb cmp = NULL;
/**
 * free block function used for freeing the internal data of the node
 * this will be writed with freeblk_init.
 */
static free_blk_cb freeblk = NULL;

/*
 * list_new(uint64_t n)
 *
 * @n - number of nodes that the linked list will contains
 *
 * Allocs a new linked list with @n number of nodes
 */
list_t *list_new(uint64_t n)
{
	if (n == 0)
		return NULL;
	list_t *l = xmalloc(sizeof(list_t));
	l->n	  = n;

	l->head = xmalloc(sizeof(node_t));
	l->tail = l->head;
	for (uint64_t i = 0; i < l->n; i++) {
		l->tail->next = xzmalloc(sizeof(node_t));
		l->tail		  = l->tail->next;
	}
	return l;
}

/*
 * list_free
 *
 * @l - ptr to ptr to a linked list
 * Free the linked list
 *
 * Free's memory from every node in the linked list.
 * After the free @l will ne NULL
 *
 */
void list_free(list_t **l)
{
	if (!*l)
		INFOEE("Could not release NULL list");

	while ((*l)->head != NULL) {
		node_t *p = (*l)->head->next;

		if ((*l)->head->blk)
			freeblk((*l)->head->blk);

		xfree((*l)->head);
		(*l)->head = p;
	}

	xfree((*l));
	*l = NULL;
}
//TODO
/*
 * list_add(list_t *l, void *blk, uin64_t sz)
 *
 * @l - ptr to a linked_list
 * @blk - ptr to block of mem
 * @sz - size of @blk
 * 
 * add's a new node that will contain @blk of mem 
 * with the size/width of @sz
 *
 *
 */
void list_add(list_t *l, void *blk, uint64_t sz)
{
	if (!l)
		INFOEE("Could not add a new node to the NULL list");

	l->tail->next	  = xzmalloc(sizeof(node_t));
	l->tail->next->blk = blk;
	l->tail->next->sz  = sz;
}

//TODO
/**
 * list_rm_n(list_t *l, uint64_t n)
 *
 * @n - n node in the linked list
 * @l - ptr to linked list
 *
 * remove the @n node from the linked list
 */
void list_rm_n(list_t *l, uint64_t n)
{
	node_t *p = l->head;
	l->n--;
	for (uint64_t i = 0; i < n - 1; i++) {
		p = p->next;
	}
	p = p->next->next;
	xfree(p->next);
}

void cmp_init(compare_cb cb)
{
	if (!cb)
		INFOEE("Could not assign NULL comapre function");
	cmp = cb;
}

void freeblk_init(free_blk_cb cb)
{
	if (!cb)
		INFOEE("Could not assign NULL free block function");
	freeblk = cb;
}

//TODO
/**
 * list_rm(list_t *l, void *blk, uint64_t sz)
 *
 * @l - linked list
 * @blk - ptr to block of mem
 * @sz - size of the block of mem
 *
 * remove the element node that has the @blk and size of @sz
 *
 */
void list_rm(list_t *l, void *blk, uint64_t sz)
{
	if (!l)
		INFOEE("Could not remove node from NULL list");
	if (!cmp)
		INFOEE("Could not remove node, please make sure you cmb_init first");
	if (!freeblk)
		INFOEE("Could not remov enode, please make sure you free_blk_init first");

	node_t **p = &l->head;
	node_t *s  = NULL;

	while (*p != NULL) {
		if (cmp((*p)->blk, blk, sz)) {
			if ((*p)->next != NULL)
				s->next = (*p)->next;
			freeblk((*p)->blk);
			xfree(*p);
			*p = NULL;
			return;
		}
		s  = *p;
		*p = (*p)->next;
	}
}
