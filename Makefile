CC = gcc
CFLAGS = -c -std=gnu11 -g -Wall 
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
	mv *.o build/
	#compile client and server objects
	$(CC) $(CFLAGS) server.c client.c
	mv *.o bin/
	
clean: 
	rm -rf build/*.o


#Dev testing options
extract:
	mv test/sctp/echo/one-to-one/client.c ~/Work/Seilfish/
	mv test/sctp/echo/one-to-one/server.c ~/Work/Seilfish/
reverse:
	mv client.c test/sctp/echo/one-to-one/
	mv server.c test/sctp/echo/one-to-one/
