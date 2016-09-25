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

#ifndef UDP_FLOOD_H
#define UDP_FLOOD_H

#include "../opts.h"

#define PK_SIZE 256

// udp_err_t type to retain errors from udp package
typedef int udp_err_t;

typedef struct udp_atk {
	// depending on the args it will generate a proc_cb
	// that will pe used to launch the attack.
	void (*proc_cb)(void *);
} udp_atk;

extern void flood(arguments args);

#endif /*UDP_FLOOD_H*/
