#include "lib/net.h"

int main_socket;
int rd_sz;
int flags;
char buffer[MAXLINE + 1];

int main(void)
{

	strncpy(buffer, "Hellow Server",12);
	buffer[12] = '\0';
	/**
	 *  This model is one to one socket
	 */
	main_socket = Socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

	initz(&server4_address, 0);
	
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	server4_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	Connect(main_socket, (SA *)&server4_address,(socklen_t)sizeof(server4_address));

	rd_sz = Sctp_sendmsg(main_socket, buffer, (size_t)strlen(buffer), (SA*)&server4_address,
						(socklen_t)(sizeof server4_address), 0, flags, 0, 0, 0);

	close(main_socket);
}
