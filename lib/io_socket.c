#include "net.h"
/*
 * Our own I/O socket functions
 */


static int byte_count;
static char *container;
static char buffer[MAXLINE]; 
static ssize_t read_buffer_new_line(int fd, void *ptr, size_t maxlen, bool level);
static ssize_t sig_read(int fd, char *ptr, bool level);
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
		buffer = buffer - number_of_bytes_written;
	}
	/*if everything is ok we should return the size that has been writen to the fd*/
	return len_buffer;
}

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
	return len_buffer - number_of_bytes_readed;
}
static
bool isInterupt(ssize_t nbytes)
{
	if( (nbytes < 0) && (errno == EINTR) )
		return true;
	else return false;
}


/**
 * Read from the fd char per char 
 * =============================
 * This method tends to be very very slow due to the
 * verbose read char per char way.
 * Also the func test if we cought a signal of type EINTR
 * and if we cought just we restart the process again until we
 * fully read everything.
 */
ssize_t
readline(int fd, void *ptr,size_t maxlen)
{
	ssize_t number_of_bytes,return_char;
	char c, *pointer;

	pointer = ptr;
	for (number_of_bytes = 0; number_of_bytes < maxlen; number_of_bytes++)
	{
		again:
			if ( (return_char = read(fd,&c,1)) == 1)
			{
				 *pointer++ = c;
				 if(c == '\n') break; /* new line is stored like fgets() */
			}
			else
				if(return_char == 0)
				{
					*pointer = 0;
					return (number_of_bytes - 1); /* EOF, n - 1 bytes were read */
				}
				else
				{
					if(errno == EINTR)
						goto again;
					return -1;
				}
	}
	*pointer = 0;
	return number_of_bytes;
}



/**
 * 
 */
ssize_t
sig_read(int fd, char *ptr, bool level)
{
		if( byte_count <= 0 )
		{
again:
				if((byte_count = read(fd,buffer,sizeof(buffer))) < 0)
				{
					if(level)
						if(isInterupt(byte_count))
							goto again;
					return -1;
				}
				else
					if(byte_count == 0)
						return 0;
			container = buffer;
		}

		byte_count--;
		*ptr++ = *container++;
		
		return 1;

}

ssize_t
read_buffer_new_line(int fd, void *ptr, size_t maxlen, bool level)
{
	ssize_t			n, rc;
	char			c, *pint;

	pint = ptr;

	for(n = 0; n<maxlen; n++)
	{
		if( (rc = sig_read(fd,&c,level)) <=0 )
		{
				*pint++ = c;
				if(c == '\n')
					break;
		}
		else
			if(rc == 0)
			{
				*pint = 0;
				return n-1;
			}
			else
				return -1;
	}//for
	
	*pint = 0;
	return n;
}

/**
 * Readline per line much more faster than char by char
 * Also we keep it save signal prone saftey to cache the signal
 * of type EINTR and restart the process again until we have readed
 * all the lines. Note that the line must not be more than our buffer.
 */
ssize_t
s_readline(int fd, void *ptr, size_t maxlen,bool level)
{
	ssize_t nbyte;
	nbyte = read_buffer_new_line(fd,ptr,maxlen,level);
	if(nbyte < 0)
		prog_error("New line buffer read error",true,errno);
	return nbyte;
}








