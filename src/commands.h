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

#ifndef COMMANDS_H
#define COMMANDS_H

#include "options.h"

// implementation type that every attack should return
// in order to consume it
typedef struct impl_t {
	void (*init)(arguments args);
	void (*clean)(void);
	void (*attack)(void);
}impl_t;


/*
 * list_attacks
 *
 * write all supported attacks in stdout
 */
extern void list_attacks(void);

/*
 * run_cmd
 * 
 * @arg - structure populated by argp_parse func
 *
 * will run the attack based on the @arg
 *
 */
extern void run_cmd(arguments arg);

/**
 * valid_attack
 *
 * @exploit - 
 *
 * checker function if the attack suplied @exploit in the cmd is valid or not.
 */
extern ATTACK_SW valid_attack(const char *exploit);

#endif /*COMMANDS_H*/
