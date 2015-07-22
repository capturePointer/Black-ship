#include "net.h"
static bool isInterupt(ssize_t nbytes);
/*
 * Our own I/O socket functions
 */

/*Write that supports 
signal non-interup process*/
ssize_t
s_write(int fd, const void *point, size_t len_buffer, bool level)
{
	size_t number_of_bytes_left = len_buffer;
	ssize_t number_of_bytes_written;
	const char* buffer = point;

	while(number_of_bytes_left > 0)
	{
		//atempt to write the data to the fd
		number_of_bytes_written = write(fd,buffer,number_of_bytes_left);
		// grant us perrmision to check
		if(level)
		{
			if(isInterupt(number_of_bytes_written))
				number_of_bytes_written = 0;
			else
				// and if it's not interupted maybe it's something else
				if(number_of_bytes_written == -1)/*error*/
				{
					echo_error("Write error",true,errno);
					return -1; 
				}
		}
		else
			/*without permission*/
			if( number_of_bytes_written == -1)
			{
				echo_error("Write error",true,errno);
				return -1;
			}
		/*if message was interupt continue sending the message */
		number_of_bytes_left = number_of_bytes_left - number_of_bytes_written;
		buffer = buffer - number_of_bytes_written;
	}
	/*if everything is ok we should return the size that has been writen to the fd*/
	return len_buffer;
}

ssize_t 
s_read(int fd, void *point, size_t len_buffer, bool level)
{
	size_t number_of_bytes_left = len_buffer;
	ssize_t number_of_bytes_readed;
	char* buffer = point;

	while(number_of_bytes_left > 0)
	{
		//atempt to read the data from the fd
		number_of_bytes_readed = read(fd,buffer,number_of_bytes_left);
		// grant us perrmision to check
		if(level)
		{
			if(isInterupt(number_of_bytes_readed))
				number_of_bytes_readed = 0;
			// and if it's not interupted maybe it's something else
			else 
				if(number_of_bytes_readed == -1) 
				{
					echo_error("Read error",true,errno);
					return -1; 
				}
		}
		else
			/*without permission*/
			//clasic test error mode
			if( number_of_bytes_readed == -1)
			{
				echo_error("Read error",true,errno);
				return -1;
			}
		/*if message was interupt continue reading the message */
		number_of_bytes_left = number_of_bytes_left - number_of_bytes_readed;
		buffer = buffer - number_of_bytes_readed;
	}
	/*if everything is ok we should return the size that has been readed from the fd*/
	return len_buffer - number_of_bytes_readed;
}
static 
bool isInterupt(ssize_t nbytes)
{
	if( (nbytes < 0) && (errno == EINTR) )
		return true;
	else return false;
}