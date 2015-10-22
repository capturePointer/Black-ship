#include "sailfish.h"
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
static char		read_buff[MAXLINE];
/**
 * Read one char
 * with safe signal handleing
 */
static ssize_t
char_read(int fd, char *ptr,bool level)
{
	// check if we have space to read more chars
	if(read_byte_count <= 0)
	{
		again:
		read_byte_count = read(fd,read_buff,MAXLINE);
		//check for perrmision sign handling
		if(level)
		{
			if(isInterupt(read_byte_count))
				goto again;
			else
				if(read_byte_count == -1)
					return -1;
		}
		else//EOF
			if(read_byte_count == 0)
			{
				return 0;
			}
			else
				if(read_byte_count == -1)
					return -1;
		read_ptr = read_buff;
	}
	read_byte_count--;
	*ptr = *read_ptr++;
	
	return 1;
}
/**
 * readline function reads using his own self buffer types
 * these buffers are static because we need to save them for every
 * execution , if we need them we can simply gem them with a method like void* _get()
 * Note that this function is not treath safety
 */
ssize_t
readline(int fd, void *point, size_t len_buffer)
{
	ssize_t n, rc;
	char	c, *ptr;
	//pass pointer to char pointer
	ptr = point;
	// for every byte of data
	for(n = 0; n<len_buffer; n++)
	{
		// if we return successfull a char 
		if( (rc= char_read(fd,&c,true)) == 1)
		{
			//our char pointer point to that char
			*ptr++ = c;
			if(c == '\n') // we found a new line
				break;
		}
		else
			//EOF
			if( rc == 0 )
			{
				//return the the numebr of bytes that were readed.
				*ptr = 0; // make our pointer point to null
				return(n - 1);
			}
			else
				return -1;
	}

	//point to null
	*ptr = 0;
	//reset read_byte_count if we perform the readline in a while/do/for loop
	//we must reset the byte_count in order to read other lines consecutive from stdin
	read_byte_count = 0;

	return n;
}

