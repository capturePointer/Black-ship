#include "net.h"
/*
 * Our own I/O socket functions
 */


/*Write that supports
signal non-interup process*/
static bool isInterupt(ssize_t nbytes);

ssize_t
s_write(int fd, const void *point, size_t len_buffer, bool level)
{
	size_t 		number_of_bytes_left = len_buffer;
	ssize_t 	number_of_bytes_written;
	const char	*buffer = point;

	while(number_of_bytes_written > 0)
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
		buffer = buffer + number_of_bytes_written;
	}
	/*if everything is ok we should return the size that has been writen to the fd*/
	return len_buffer;
}
/**
 * ssize_t s_read 
 * reades until we hit the maximum len_buffer
 * if in the fd we had writed 255 bits and our len_buffer is 2000 bis length
 * we will read 255 bits and we will stay in blocking mode
 * because the input is way to small of the length output we've expected
 * the s_read expects that you written in the fd  exact the len_buffer of our func
 */
ssize_t
s_read(int fd, void *point, size_t len_buffer, bool level)
{
	size_t      number_of_bytes_left = len_buffer;
	ssize_t 	number_of_bytes_readed;
	char* 		buffer = point;

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
	return len_buffer - number_of_bytes_left;
}
static
bool isInterupt(ssize_t nbytes)
{
	if( (nbytes < 0) && (errno == EINTR) )
		return true;
	else return false;
}


static int		read_byte_count;
static char		*read_ptr;
static char		read_buf[MAXLINE];
/**
 * Read one char
 * with safe signal handleing
 */
static ssize_t
char_read(int fd, char *ptr,bool level)
{
	if(read_byte_count <= 0)
	{
		again:
		read_byte_count = read(fd,read_buf,MAXLINE);
		if(level)
		{
			if(isInterupt(read_byte_count))
				goto again;
			else
				if(read_byte_count == -1)
					return -1;
		}
		else
			if(read_byte_count == 0)
			{
				return 0;
			}
			else
				if(read_byte_count == -1)
					return -1;
		read_ptr = read_buf;
	}
	read_byte_count -- ;
	*ptr = *read_ptr++;
	
	return 1;
}
/**
 *
 */
ssize_t
readline(int fd, void *point, size_t len_buffer)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = point;

	for(n = 0; n<len_buffer; n++)
	{
		if( (rc= char_read(fd,&c,true)) == 1)
		{
			*ptr++ = c;
				if(c == '\n')
					break;
		}
		else
			if( rc == 0 )
			{
				*ptr = 0;
				return n-1;
			}
			else
				return -1;
	}

	*ptr = 0;
	return n;
}

