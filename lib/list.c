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

//TODO
list_t *list_new(uint64_t n)
{
	if (n == 0) 
		return NULL;
	list_t *l = xmalloc(sizeof(list_t));
	l->n = n;

	l->head = xmalloc(sizeof(node_t));
	l->tail = l->head;
	for (uint64_t i=0; i<l->n; i++) {
		l->tail->next = xzmalloc(sizeof(node_t));	
		l->tail = l->tail->next;
	}
	return l;
}

void list_free(list_t **l)
{
	if (!*l)
		INFOEE("Could not release NULL list");
	
	while((*l)->head != NULL) {
		node_t *p = (*l)->head->next;
		xfree((*l)->head);
		(*l)->head = p;
	}
	
	xfree((*l));
	*l = NULL;
}
