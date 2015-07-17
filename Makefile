CC = gcc
CFLAGS = -c -std=c99 -g -Wall -o
LIB_PATH = lib/
BIN_PATH = bin/

OBJECTS_LIBRARY = bin/error.o bin/wrapper_functions.o
MAIN_FILES = bin/client.o


all: client.c $(LIB_PATH)error.c 
	#Compile
	$(CC) $(CFLAGS) $(BIN_PATH)client.o client.c
	$(CC) $(CFLAGS) $(BIN_PATH)error.o $(LIB_PATH)error.c 
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_functions.o $(LIB_PATH)wrapper_functions.c
	#Link
	$(CC) -o $(BIN_PATH)client $(OBJECTS_LIBRARY) $(MAIN_FILES)

	
clean:
	rm -rf bin/*.o
