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
// thic cmd tool uses argp as it's parser mechanism for the
// command line arguments
#include <argp.h>
#include <argz.h>

#include "blackship.h"
#include "opts.h"
#include "cmds.h"

// here we should treat all the command line parsing
// logic and init all the other func with in this
// commnad line
int main(int argc, char **argv)
{
	arguments arg;
	memset(&arg, 0, sizeof(arguments));

	/* struct arguments argu; */
	// init function to parse all argc to argp logic
	if (argp_parse(&argp, argc, argv, 0, 0, &arg) == 0) {
		// so after the parsing is done this should end
		// the execution of the program and sent SIGCHLD
		// to the parent process, and all child processes are
		// assigned a new parrent, also all open streams are closed
		printf("argumnets\n");
		printf("host : %s\n", arg.host);
		printf("port: %d\n", arg.port.n);
		printf("random: %d\n", (arg.port.random) ? 1 : 0);
		printf("range-ports \n");
		printf("low: %d , high: %d\n", arg.port.low, arg.port.high);
		printf("list_attacks: %d\n", (arg.list_attacks) ? 1 : 0);
	}
	exit(EXIT_FAILURE);
}
