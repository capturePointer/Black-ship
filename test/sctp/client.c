#include	"lib/net.h"

#define	SCTP_MAXLINE	800


	int sock_fd;
	struct sctp_event_subscribe evnts;
	int echo_to_all=0;// By default the flag is not set

void
sctpstr_cli_echoall(FILE *fp, int sock_fd, struct sockaddr *to, socklen_t tolen)
{
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[SCTP_MAXLINE], recvline[SCTP_MAXLINE];
	socklen_t len;
	int rd_sz, strsz;
	size_t i;
	int msg_flags;


	/**
	 * As before, the client initializez the sri
	 * structure used to set up the stream it will be sending and reciving from
	 * in addition, the client zeros out the data buffer from which it will
	 * collect user input. Then, the client enters the main loop, once again
	 * blocking on user input
	 */
	memset(sendline, 0, sizeof(sendline));
	memset(&sri, 0, sizeof(sri));	
	while (fgets(sendline, SCTP_MAXLINE - 9, fp) != NULL) 
	{
		/**
		 * The client sets p the message size and the deletes the newline
		 * character that is the end of the buffer(if andy).
		 */
		strsz = strlen(sendline);
		if(sendline[strsz-1] == '\n') 
		{
			sendline[strsz-1] = '\0';
			strsz--;
		}
		
		//FIRST FOR
		/**
		 * The client sends the message using the sctp_sendmsg function,
		 * sending the whole buffer of SCTP_MAXLINE bytes.Before sending 
		 * the message it appends the string ".msg." and the stream number
		 * so that we can observe the order of the arriving messages.In this
		 * way, we can compare the arrival order to the order in which the 
		 * client sent the actual messages.Note also the client
		 * sends the messages to a set number of streams without regard to how
		 * many were actually set up.It is possible that one or more of the sends
		 * may fail if the peer negotiates the number of streams downward.
		 */
		for(i=0;i<SERV_MAX_SCTP_STRM;i++) 
		{
			snprintf(sendline + strsz, sizeof(sendline) - strsz,
				".message.%lu 1", i);

			Sctp_sendmsg(sock_fd, sendline, sizeof(sendline), 
						to, tolen, 0, 0,i,0, 0);

			snprintf(sendline + strsz, sizeof(sendline) - strsz,
					".message.%lu 2", i);
			
			Sctp_sendmsg(sock_fd, sendline, sizeof(sendline), 
						to, tolen, 0, 0,i,0, 0);
 
		}
	    //SECOND FOR
	    /**
	     * This code has the potential to fail if the send or recive windows
	     * are too small. If the peer's receive windows is to small, it is possible that the 
	     * client will block.Since the client does not read any information until all of its sends
	     * are complete, the server could also potentially block while waiting for the client
	     * to finish reading the responses the server already sent.the result of such scenario
	     * would be a dreadlock of the two endpoints.This code is not meant to be scalable, but 
	     * instread to ilustrate streams and head-of-line blocking in a simple, straightforward manner
	     */
		
		/**
		 * We now block , reading all the response messages from our server
		 * and displaying each as we did before.After the last
		 * message is read, the client loops back for more user input.
		 */
		for(i=0;i<SERV_MAX_SCTP_STRM;i++) 
		{
			len = sizeof(peeraddr);
			rd_sz = Sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
				     (SA *)&peeraddr, &len,
				     &sri,&msg_flags);
			
			printf("From stream:%d seq:%d (assoc:0x%x):",
				sri.sinfo_stream,sri.sinfo_ssn,
				(u_int)sri.sinfo_assoc_id);
			printf("%.*s\n",rd_sz, recvline);
			
			rd_sz = Sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
				     (SA *)&peeraddr, &len,
				     &sri,&msg_flags);
			
			printf("From stream:%d seq:%d (assoc:0x%x):",
				sri.sinfo_stream,sri.sinfo_ssn,
				(u_int)sri.sinfo_assoc_id);
			printf("%.*s\n",rd_sz, recvline);
 
		}
	}
}


void
sctpstr_cli(FILE *fp, int sock_fd, struct sockaddr *to, socklen_t tolen)
{
	struct sockaddr_in peeraddr;
	struct sctp_sndrcvinfo sri;
	char sendline[MAXLINE], recvline[MAXLINE];
	socklen_t len;
	int out_sz,rd_sz;
	int msg_flags;

	/**
	 * The clients starts by clearing the sctp_sndrcvinfo structure,
	 * sri. The client then enters a loop that reads from the fp passed 
	 * by our caller with a blocking call to fgets.The main program passes
	 * stdin to this func  so user input is read and processed in the loop 
	 * until the terminal EOF character(CONTROL+D) is typed by the user.
	 * The user action ends the function and causes a return to the caller.
	 */
	memset(&sri, 0, sizeof(sri));
	while (fgets(sendline, MAXLINE, fp) != NULL) 
	{
		if(sendline[0] != '[') 
		{
			printf("Error, line must be of the form '[streamnum]text'\n");
			continue;
		}
	    
	    /**
	     * The client translates the user request stream found in the input
	     * into the sinfo_stream field in the sri structure
	     */
		sri.sinfo_stream = strtol(&sendline[1],NULL,0);//convert string into long integer
		/**
		 * After initializing the appropiate lengths of the address and the size of the
		 * actual user data, the client sends the message using the sctp_sendmsg funct
		 */
		out_sz = strlen(sendline);
		Sctp_sendmsg(sock_fd, sendline, out_sz, 
			     to, tolen, 
			     0, 0,
			     sri.sinfo_stream,
			     0, 0);

		/**
		 * The client now blocks and waits for the echoed message from the server
		 */
		len = sizeof(peeraddr);
		rd_sz = Sctp_recvmsg(sock_fd, recvline, sizeof(recvline),
			     (SA *)&peeraddr, &len,
			     &sri,&msg_flags);
		
		/**
		 * The client displays the returned message echoed to it displaying stream
		 * number, stream sequence number as well as the text message.After displaying
		 * the message, the client loops back to get another request from the user.
		 */
		printf("From stream:%d seq:%d (assoc:0x%x):",
		       sri.sinfo_stream, sri.sinfo_ssn,
		       (u_int)sri.sinfo_assoc_id);
		
		printf("%.*s",rd_sz, recvline);
	}
}


struct sctp_initmsg initm;

int
main(int argc, char **argv)
{
	argc_length(argc,2);
	/*Test if the number of arguments is  higher than 2*/
	if(argc > 2) 
	{
		printf("Echoing messages to all streams\n");
		echo_to_all = 1;
	}
    /*
     * The client validates the arguments passed to it.First, the client 
     * verifies that the caller provided a host to send messages to.
     * It then checks if the "echo to all" option is being enabled,
     * finally the client creates an SCTP one-to-many-style socket
     * */
	sock_fd = Socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
	
	sctp_set_number_streams(&sock_fd, &initm, SERV_MAX_SCTP_STRM);

	/* Fill the server4_addres struct with 0*/
	memset(&server4_address,0 ,sizeof(server4_address));
    /* Complete the struct with the require protocol settings */
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = htons(PORT);
	server4_address.sin_addr.s_addr = htonl(INADDR_ANY);
	Inet_pton(AF_INET, argv[1], &server4_address.sin_addr);

	/* Fill the evets struct with 0 */
	memset(&evnts,0, sizeof(evnts));
	/**
	 * Set the socket with the right options as the server
	 */
	evnts.sctp_data_io_event = 1;
	Setsockopt(sock_fd,IPPROTO_SCTP, SCTP_EVENTS,
		   &evnts, sizeof(evnts));

	/**
	 * If the echo_to_all flag is not set, the client calls the sctpstr_cli
	 * function.If the echo_to_all flag is set, the client calls the
	 * sctpstr_cli_echoall function.
	 */
	if(echo_to_all == 0)
		sctpstr_cli(stdin,sock_fd,(SA *)&server4_address,
					sizeof(server4_address));
	else
		sctpstr_cli_echoall(stdin,sock_fd,(SA *)&server4_address,
							sizeof(server4_address));
	char byemsg[7];
	strcpy(byemsg,"goodbye");
	Sctp_sendmsg(sock_fd, byemsg, strlen(byemsg),(SA *)&server4_address,sizeof(server4_address),0, SCTP_ABORT, 0, 0, 0);
	/* close the socket*/
	Close(sock_fd);

}
