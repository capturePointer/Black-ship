#include "net.h"
static void error_handler(const char*,int);

void 
argc_length(int argc,int expect)
{
	if(argc <expect)
	{
		error_handler("None argumnets passed.\nPlease pass more arguments\n",-1);
		
		exit(EXIT_FAILURE);
	}
	
}
void
error_exit(const char *message)
{
		error_handler(message,-1);
		exit(EXIT_FAILURE);
}
/*
 * Error function to handle special function that must return
 */
void * 
error_system_exit(const char *message,void* flag)
{
		error_handler(message,flag);
		return flag;
}


/*Static function error_handler forbit other units to access it
 * This make's sure that no other file has access to encapsulate 
 * our error_hadler function
 */
static void 
error_handler(const char *message,int flag)	
{
	size_t message_length = sizeof(message);
	char *standard_message= "ERROR : ";
	/*Print the explicit version error*/
	fwrite(message,1,message_length,stdout);
	
	/*System critical error*/
	/*Print standard error set by errno*/
	if(flag == -1)
		snprintf(standard_message,strlen(standard_message)," %s",strerror(errno));

}
