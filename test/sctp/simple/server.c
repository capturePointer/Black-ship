#include "sctp.h"

int main(void)
{
	/**
	 *  This model is one to one socket
	 */
	main_socket = Socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

	setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	memset(&server4_address, 0, sizeof(struct sockaddr_in));
	
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(main_socket, (SA *)&server4_address,(socklen_t)sizeof(server4_address));

	memset(&initmsg, 0,sizeof(initmsg));
	memset(&sndrcvinfo, 0,sizeof(sndrcvinfo));
	memset(&event, 0, sizeof(event));
    /**
     * Set the number of streams at 5
     * And the number of attempts 4
     * Also note that our sctp_set_number_streams is setting 
     * with the setsockopt api function
     * And after the call aour initmsg struct is updated
     */

	sctp_set_number_streams(main_socket, &initmsg, 5,4);

	Listen(main_socket, LISTENQ);

	while(true)
	{
		memset(buffer, 0, sizeof(buffer));
		printf("Awaiting a new connection ... \n");
		len = sizeof(client4_address);
		client_socket = Accept(main_socket, (SA *)&client4_address, &len);

		rd_sz = Sctp_recvmsg(client_socket, buffer, sizeof(buffer), 
					(SA *)&client4_address, 0, &sndrcvinfo, &flags);
		//
		//rd_sz = Recv(client_socket, buffer, sizeof(buffer), 0);
		printf("Data: %s\n",buffer);
		close(client_socket);
	}
}
