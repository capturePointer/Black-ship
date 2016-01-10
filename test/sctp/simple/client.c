#include "sctp.h"

int main(int argc, char *argv[])
{
	argc_length(argc,2);

	strncpy(buffer, "Hellow Server", 13);
	buffer[13] = '\0';

	/**
	 *  This model is one to one socket
	 */
	main_socket = Socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

	memset(&server4_address, 0, sizeof(struct sockaddr_in));
	
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);
	Connect(main_socket, (SA *)&server4_address, sizeof(server4_address));

	printf("buffer = %s\n", buffer);

    rd_sz = Send(main_socket, &buffer, sizeof(buffer), 0);
	close(main_socket);
}
