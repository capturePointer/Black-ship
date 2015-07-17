#include "lib/net.h"

int main_socket;

int
main(int argc, char *argv[])
{
	argc_length(argc);
	
	main_socket = Socket(AF_INET,SOCK_STREAM,0);
	
	memset(&server4_address,0,sizeof(server4_address));
	
	server4_address.sin_family = AF_INET;
	server4_address.sin_port = PORT;

	Inet_pton(AF_INET,argv[1],&server4_address.sin_addr);

}
