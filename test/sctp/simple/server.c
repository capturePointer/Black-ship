#include "lib/net.h"


int main_socket, client_socket;
int rd_sz, wr_sz, flags;
int i;


struct sctp_initmsg initmsg;
struct sctp_event_subscribe event;
struct sctp_sndrcvinfo sndrcvinfo;


char buffer[MAXLINE + 1];
size_t len;
int reuse;
int main(void)
{
	/**
	 *  This model is one to one socket
	 */
	main_socket = Socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

	setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	initz(&server4_address, 0);
	
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(main_socket, (SA *)&server4_address,(socklen_t)sizeof(server4_address));


    initz(&initmsg, 0);
    initz(&sndrcvinfo, 0);
    initz(&event, 0);
    /**
     * Set the number of streams at 5
     * And the number of attempts 4
     * Also note that our sctp_setNumber_streams is setting 
     * with the setsockopt api function
     * And after the calll aour initmsg struct is updated
     */
	sctp_set_number_streams(main_socket, &initmsg, 5,4);

    Listen(main_socket, LISTENQ);

    while(true)
	{
       initz(buffer, 0);
	   printf("Awaiting a new connection ... \n");
	   len = sizeof(client4_address);
	   client_socket = Accept(main_socket, (SA *)&client4_address, (socklen_t *)&len);

	   rd_sz = Sctp_recvmsg(main_socket, buffer, sizeof(buffer), (SA *)&client4_address,
							0, &sndrcvinfo, &flags);
	   printf("Data: %s\n",buffer);
	   close(client_socket);
	}
}
