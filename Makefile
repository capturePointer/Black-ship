CC = gcc
CFLAGS = -c -std=c99 -g -Wall -o
LIB_PATH = lib/
BIN_PATH = bin/

OBJECTS_LIBRARY = bin/error.o bin/wrapper_socket_functions.o bin/wrapper_stdio.o bin/wrapper_convert.o
MAIN_FILES = bin/client.o


all:
	#Compile MAIN_FILES
	$(CC) $(CFLAGS) $(BIN_PATH)client.o client.c
	#Compile LIB_FILES
	$(CC) $(CFLAGS) $(BIN_PATH)error.o $(LIB_PATH)error.c 
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_socket_functions.o $(LIB_PATH)wrapper_socket_functions.c
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_stdio.o $(LIB_PATH)wrapper_stdio.c
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_convert.o $(LIB_PATH)wrapper_convert.c
	
	#Link
	$(CC) -o $(BIN_PATH)client $(OBJECTS_LIBRARY) $(MAIN_FILES)

	
clean:
	rm -rf bin/*.o
