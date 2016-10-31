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
#ifndef MEM_H
#define MEM_H

#include <stdio.h>

/**
 * mem_malloc 
 *
 * @sz - the size of mem you wish to alloc
 *
 * wrapper for the std lib allocation funciton malloc
*/
extern void *mem_malloc(size_t sz);

/**
 * mem_calloc 
 *
 * @n -  number of elements to be allocated
 * @sz - the size of elements
 * 
 * wrapper for the std lib allocation function calloc
 *
 */
extern void *mem_calloc(size_t n, size_t sz);

/**
 * mem_zmalloc 
 *
 * sz - the size of the block of mem you wish to alloc
 * 
 * wrpper for the std lib function malloc but it zeros the mem also
 */

extern void *mem_zmalloc(size_t sz);
/**
 * mem_free 
 * 
 * @blk- ptr to that block of mem
 *
 * wrapper for the stdlib free function
 *
 */
extern void mem_free(void *blk);


/**
 * just friendly renames of the funcs
 * to provide a better interface
 */
#define xmalloc(sz) \
	mem_malloc((sz))

#define xcalloc(count, n) \
	mem_calloc((count))

#define xzmalloc(sz) \
	mem_zmalloc((sz))

#define xfree(ptr) \
	mem_free((ptr))

#endif /*MEM_H*/
