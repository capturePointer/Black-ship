#include	"../../../../lib/sailfish.h"

	int sock_fd,msg_flags;
	char readbuf[BUFFSIZE];
	struct sctp_sndrcvinfo sri;
	struct sctp_event_subscribe evnts;
	int stream_increment=1;
	socklen_t len;
	size_t rd_sz;
	struct sctp_initmsg initm;




int
main(int argc, char **argv)
{

	if(argc == 2)
		stream_increment = atoi(argv[1]);
	/*Sctp socket one to many created*/ 
    sock_fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
	
	/*
     * Sets the actual number of streams that sctp server will support
     * Note that SERV_MORE_STRMS_SCTP is just a macro, that macro you 
     * could change or create another one.
     */
	//sctp_set_number_streams( sock_fd, &initm,SERV_MORE_STRMS_SCTP);
	
	/*Fill the server struct with 0*/
	initz(&server4_address, 0);
	/* Fill the addres struct with specific protocol dependent info*/
	server4_address.sin_family = AF_INET;
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server4_address.sin_port = htons(PORT);

	/*Bind our socket to a interface*/
	Bind(sock_fd, (SA *) &server4_address, sizeof(server4_address));
	
	/*Fill sctp_event_subscribe struct with 0*/
	initz(&evnts, 0);

	/*
	* The server changes the subscription for the one-to-many SCTP sockets.
	* The server subscribes to just the sctp_data_io_event,witch will allow 
	* the server to see the number_sctp_sndrcvinfo structure.From this structure 
	* the server can determine the stream number on with the messsage arrived.
	*/

	evnts.sctp_data_io_event = 1;
	Setsockopt(sock_fd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts));

	/*Make the socket listen and specify our queue*/
	Listen(sock_fd, LISTENQ);

/*
	* This program runs 4 ever until the user shuts it down
	* with an external signal like CTRL+D
	* */
for ( ; ; )
{
	/*
		* The server initializez the size of the client socket address struct
		* then blocks while waiting for a message from any remote peer.
		*/
	len = sizeof(struct sockaddr_in);
	rd_sz = Sctp_recvmsg(sock_fd, readbuf, sizeof(readbuf),
				(SA *) &client4_address, &len,
				&sri, &msg_flags);
	/*
		* Whem a message arrives, the server checks the stream_increment flag
		 * to see if it should increment the stream number.If the flag is set
		 * (no arguments were passed on the command line),theserver
		 * increments the stream  number of the message.If that number grows larget than
		 * or equal to the maximum streams, which is obtained by calling our internal
		 * function call sctp_get_number_streams, the server resets the stream to 0.
		 */
		if(stream_increment)
		{
			sri.sinfo_stream++;
			if(sri.sinfo_stream >= sctp_get_number_streams(sock_fd,(SA *)&client4_address, 
															len, &sri) )
			{
				sri.sinfo_stream = 0;
			}
		}
		/*
		 * The server sends back the message using the playload protocol ID, 
		 * flags, and the possibly modified the stream  number from the sri structure
		 * notice that the server does not want association notification, so it
		 * disables all events that would pass messages up the socket buffer.
		 * The server relies on the information in the sctp_sndrcvinfo structure
		 * and the returned address found in client4_address to locate the peer
		 * association and return the echo.
		 */
		 Sctp_sendmsg(sock_fd, readbuf, rd_sz, (SA *)&client4_address, len,
					  sri.sinfo_ppid, sri.sinfo_flags, sri.sinfo_stream, 0, 0);
	}
}
