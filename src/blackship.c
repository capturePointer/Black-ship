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

#include <lib/err.h>

#include "opts.h"
#include "cmds.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		return 0;

	error_t err = 0;
	arguments arg;
	memset(&arg, 0, sizeof(arguments));

	err = argp_parse(&argp, argc, argv, 0, 0, &arg);
	if (err == 0) {
		run_cmd(arg);
	} else if (err == ARGP_KEY_ERROR) {
		err_dump();
	}
}
