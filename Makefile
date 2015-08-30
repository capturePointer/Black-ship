CC = gcc
CFLAGS = -lsctp -c -std=gnu11 -g -Wall -o
LIB_PATH = lib/
BIN_PATH = bin/

OBJECTS_LIBRARY = bin/error.o bin/signal.o bin/io_socket.o bin/wrapper_socket_functions.o bin/wrapper_stdio.o bin/wrapper_convert.o bin/wrapper_io_socket.o bin/wrapper_unix.o bin/handlers.o bin/miscellaneous.o bin/protocol.o
CLIENT_FILES = bin/client.o
SERVER_FILES = bin/server.o

all:

	#Compile MAIN_FILES

	$(CC) $(CFLAGS) $(BIN_PATH)client.o client.c
	$(CC) $(CFLAGS) $(BIN_PATH)server.o server.c

	#Compile LIB_FILES

	$(CC) $(CFLAGS) $(BIN_PATH)handlers.o $(LIB_PATH)handlers.c
	$(CC) $(CFLAGS) $(BIN_PATH)signal.o $(LIB_PATH)signal.c
	$(CC) $(CFLAGS) $(BIN_PATH)error.o $(LIB_PATH)error.c
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_socket_functions.o $(LIB_PATH)wrapper_socket_functions.c
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_stdio.o $(LIB_PATH)wrapper_stdio.c
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_convert.o $(LIB_PATH)wrapper_convert.c
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_io_socket.o $(LIB_PATH)wrapper_io_socket.c
	$(CC) $(CFLAGS) $(BIN_PATH)io_socket.o $(LIB_PATH)io_socket.c
	$(CC) $(CFLAGS) $(BIN_PATH)wrapper_unix.o $(LIB_PATH)wrapper_unix.c
	$(CC) $(CFLAGS) $(BIN_PATH)miscellaneous.o $(LIB_PATH)miscellaneous.c
	$(CC) $(CFLAGS) $(BIN_PATH)protocol.o $(LIB_PATH)protocol.c
	#Link CLIENT

	$(CC) -o $(BIN_PATH)client $(OBJECTS_LIBRARY) $(CLIENT_FILES)

	#Link SERVER

	$(CC) -o $(BIN_PATH)server $(OBJECTS_LIBRARY) $(SERVER_FILES)

	#Remove unwanted objects

	rm -rf bin/*.o
