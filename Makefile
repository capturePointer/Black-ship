CC = gcc
CFLAGS =  -c -g -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align -Wmissing-prototypes -Wmissing-declarations -Winline -Wlong-long -Wuninitialized -Wconversion -Wstrict-prototypes -Werror -Wno-variadic-macros
LINK_FLAGS = -lsctp
LIBS = $(wildcard lib/*.c)
OBJECT = $(wildcard build/*.o)
BUILD_PATH = build/

link: compile
	cd build/
	$(CC) $(LINK_FLAGS) $(OBJECT) bin/server.o  -o bin/server
	$(CC) $(LINK_FLAGS) $(OBJECT) bin/client.o  -o bin/client
	rm -rf bin/*.o
compile:
	$(CC) $(CFLAGS) $(LIBS)
	mv *.o $(BUILD_PATH)
	#compile client and server objects
	$(CC) $(CFLAGS) server.c client.c
	mv *.o bin/
clean:
	rm -rf build/*.o
	rm -rf bin/*
