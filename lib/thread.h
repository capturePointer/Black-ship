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
#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

#include <lib/info.h>

// thread_t type that holds a trhead id and his mutex
typedef thread_t struct {
	pthread_t id;
	pthread_mutex_t *mu;
}thread_t;


extern void thread_create(void);

#endif
