#include "../../../lib/sailfish.h"

int main_socket;
ssize_t rd_sz;
int flags;
char buffer[MAXLINE + 1];
int main(int argc, char *argv[])
{
	argc_length(argc,2);

	strncpy(buffer, "Hellow Server",12);
	buffer[12] = '\0';
	/**
	 *  This model is one to one socket
	 */
	main_socket = Socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

	initz(&server4_address, 0);
	
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);

	Connect(main_socket, (SA *)&server4_address, sizeof(server4_address));

    rd_sz = Send(main_socket, &buffer, sizeof(buffer), 0);
	close(main_socket);
}
