# Copyright [2016] [hoenir]
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

TARGET=blackship
# use gcc to compile the hole project
CC=gcc

# set here the flags to check and enable different gcc options
FLAGS=-Wall -g -c -std=gnu11
SRC=$(wildcard *.c) $(wildcard util/*.c) $(wildcard err/*.c)
BIN=blackship

link:compile
	$(CC) $(wildcard *.o) -o $(BIN)
compile:
	$(CC) $(FLAGS) $(SRC)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf blackship

.PHONY: test
test:
	@echo ${RED}'<========RUNING TESTS========>'${NC}
	cd err/test/ && make --silent && ./err_test
