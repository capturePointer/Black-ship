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
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"
#include "mem.h"

#define ALLOC_RETRY 5

void *mem_malloc(size_t n)
{
	void *p   = NULL;
	uint8_t i = 0;

	if (!n)
		INFOEE("[ERROR] Invalid size for alloc memory");

// alloc block of mem
retry:
	p = malloc(n);
	if ((!p) && (i < ALLOC_RETRY)) {
		i++;
		goto retry;
	}

	if ((i == ALLOC_RETRY) && (errno == ENOMEM))
		INFOEE("[ERROR] Allocation failed, out of memory");
	return p;
}

void *mem_calloc(size_t count, size_t n)
{
	void *p   = NULL;
	uint8_t i = 0;

	if (!n)
		INFOEE("[ERROR] Invalid size for alloc memory");

// alloc block of mem
retry:
	p = calloc(count, n);
	if ((!p) && (i < ALLOC_RETRY)) {
		i++;
		goto retry;
	}

	if ((i == ALLOC_RETRY) && (errno == ENOMEM))
		INFOEE("[ERROR] Allocation failed, out of memory");
	return p;
}

void *mem_zmalloc(size_t n)
{
	void *p   = NULL;
	uint8_t i = 0;

	if (!n)
		INFOEE("[ERROR] Invalid size of alloc memory");

// alloc block of mem
retry:
	p = malloc(n);

	if ((!p) && (i < ALLOC_RETRY)) {
		i++;
		goto retry;
	}

	if ((i == ALLOC_RETRY) && (errno == ENOMEM))
		INFOEE("[ERROR] allocation failed out of memory");

	// set the entire block of mem to 0
	memset(p, 0, n);
	return p;
}

void mem_free(void *p)
{
	if (!p)
		INFOEE("[ERROR] Invalid pointer passed to free");
	free(p);
}
