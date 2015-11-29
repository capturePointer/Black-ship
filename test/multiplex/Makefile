CC = gcc
CFLAGS = -c -std=gnu11 -g -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wmissing-prototypes -Wmissing-declarations -Winline -Wlong-long -Wuninitialized -Wconversion -Wstrict-prototypes -Werror -Wno-variadic-macros
LINK_FLAGS = -lsctp
LIBS = $(wildcard ../../lib/*.c)
OBJECT = $(wildcard ../../build/*.o)
BUILD_PATH = ../../build/

link: compile
	# First
	$(CC) $(LINK_FLAGS) $(OBJECT) ../../bin/server.o  -o server
	$(CC) $(LINK_FLAGS) $(OBJECT) ../../bin/client.o  -o client
	rm -rf ../../bin/*.o
compile:
	$(CC) $(CFLAGS) $(LIBS)
	mv *.o $(BUILD_PATH)
	# First
	$(CC) $(CFLAGS) server.c client.c
	mv *.o ../../bin/
clean:
	rm -rf ../../build/*.o
	rm -rf ../../bin/*
	rm -rf *.o
	rm -rf client server
