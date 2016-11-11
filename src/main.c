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

#include <argz.h>
#include <sys/types.h>
#include <unistd.h>

#include <lib/info.h>

#include "commands.h"
#include "options.h"

int main(int argc, char **argv)
{
	if ((getuid() | getgid()) != 0) {
		STATUS("The program must be run with root privileges");
		return 0;
	}

	if (argc < 2) {
		DEBUG("No arguments passed on the command line");
		return 0;
	}

	arguments arg;
	memset(&arg, 0, sizeof(arguments));

	error_t err = argp_parse(&argp, argc, argv, 0, 0, &arg);
	if (!err)
		run_cmd(arg);
	else if (err == ARGP_KEY_ERROR)
		INFOEE("Bad argument input");
}
