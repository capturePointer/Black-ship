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

#include <stdlib.h>

#include "mt.h"


// Parameters of 32-bit MT19937
#define UPPER_MASK 0x9D2C5680			// UPPER BITMASK b
#define LOWER_MASK 0xEFC60000			// LOWER BITMASK c
#define N 624							// degree of recurrence
#define W 32							// word size (in number of bits)
#define R 31
#define M 397
#define A 99083B0DF
#define U 11
#define S 7
#define T 15
#define	L 18







