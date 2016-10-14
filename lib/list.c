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
 * Allocs a new empty linked list
 */
list_t *list_new(cmp_fn_t cmp, free_fn_t free)
{
	list_t *list = xzmalloc(sizeof(*list));
	list->cmp = cmp;
	list->free = free;
	return list;
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
		INFOEE("Could not use free NULL fn");

	while ((*l)->head != NULL) {
		node_t *p = (*l)->head->next;

		if ((*l)->head->blk)
			(*l)->free((*l)->head->blk);

		xfree((*l)->head);
		(*l)->head = p;
	}

	xfree((*l));
	*l = NULL;
}
