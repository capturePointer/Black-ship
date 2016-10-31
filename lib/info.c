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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"

inline void inf(const char *msg, const char *file, int line, INFO_OPT opt)
{
	switch (opt) {
	case ERR_EX:
		fprintf(stderr, KRED "[ ERR ]" RESET "  %s - %s %d\n", msg, file, line);
		exit(EXIT_FAILURE);
	case WA:
		fprintf(stdout, KYEL "[ WAR ]" RESET "  %s\n", msg);
		break;
	case INF:
		fprintf(stdout, KBLU "[ INFO ]" RESET " %s %s %d\n", msg, file, line);
		break;
	case STAT:
		fprintf(stdout, "[ *** ] %s\n", msg);
		break;
	case DBG:
		fprintf(stdout, KGRN "[ DEBUG ]" KWHT "%s:%d - %s\n" RESET, file, line, msg);
		break;
	default:
		fprintf(stdout, "%s\n", msg);
		break;
	}
}
