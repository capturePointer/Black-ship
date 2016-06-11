CC=gcc
FLAGS=-Wall -g -c
TESTS=$(wildcard *.c)
OBJ=$(wildcard *.o)
ERR_LIB= $(wildcard ../*.c)
UTIL_LIB = $(wildcard ../../util/*.c)
BIN=err_test
LINKFLAGS=-lcmocka

link:compile
	$(CC) $(LINKFLAGS) $(OBJ) -o $(BIN)
	#make run
compile:
	$(CC) $(FLAGS) $(TESTS) $(ERR_LIB) $(UTIL_LIB)
run:
	./$(BIN)
clean:
	rm -rf *.o
run:
	./err_test
	valgrind -q ./err_test

