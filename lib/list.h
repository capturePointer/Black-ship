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

#include <stdint.h>

typedef struct data_t {
	void *blk;
	uint64_t sz;
}data_t;

typedef struct node_t {
	data_t data;
	struct node_t *next;
}node_t;

typedef struct list_t{
	node_t *head;
	node_t *tail;
	uint64_t n;
}list_t;

extern list_t *list_new(uint64_t);
extern void list_free(list_t **);
