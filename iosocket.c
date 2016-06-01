#include <errno.h>

#include "error.h"
#include "iosocket.h"
#include "sail.h"

//
static inline bool is_interupted(ssize_t nbytes, int err);

/**
 * ssize_t sail_write 
 * func will write the socket and will block if the number
 * of bytes left is not had been written.
 * sail_write supports I/O on socket catching an interupt singal
 * and continues to write to the socket.
 * this function should be used to be sure you have written all the memory
 * to that socket without been interupted
 * before the func was called s_write from safe write
 */
ssize_t sail_write(int fd, const void *point, size_t len_buffer, bool level)
{
	// save the number of bytes left
	size_t n_of_bytes_left = len_buffer;

	// this var will hold the n
	ssize_t n_of_bytes_written;

	// point to the begining of the buffer
	const char *buffer = point;

	while (n_of_bytes_left > 0) {
		// atempt to write the data to the fd
		n_of_bytes_written = write(fd, buffer, n_of_bytes_left);
		// grant us perrmision to check
		if (level) {
			if (is_interupted(n_of_bytes_written, errno))
				n_of_bytes_written = 0;
			else
				// and if it's not interupted maybe it's something else
				if (!n_of_bytes_written) {
				infoe_exit("Write error", true, errno);
				return -1;
			}
		}					 // without permision
		else if (!n_of_bytes_written) {
			infoe_exit("Write error", true, errno);
			return -1;
		}

		/* if message was interupt continue sending the message */
		n_of_bytes_left = n_of_bytes_left - (size_t)n_of_bytes_written;
		buffer += n_of_bytes_written;
	}

	/* if everything is ok we should return the size that has been writen to the fd*/
	return (ssize_t)len_buffer;
}

/**
 * ssize_t sail_read 
 * reades until we hit the maximum len_buffer
 * if in the fd we had writed 255 bits and our len_buffer is 2000 bis length
 * we will read 255 bits and we will stay in blocking mode
 * because the input is way to small of the length output we've expected
 * the s_read expects that you written in the fd  exact the len_buffer of our func
 */
ssize_t sail_read(int fd, void *point, size_t len_buffer, bool level)
{
	size_t n_of_bytes_left = len_buffer;
	ssize_t n_of_bytes_readed;
	char *buffer = point;

	while (n_of_bytes_left > 0) {
		//atempt to read the data from the fd
		n_of_bytes_readed = read(fd, buffer, n_of_bytes_left);
		// grant us perrmision to check
		if (level) {
			if (is_interupted(n_of_bytes_readed, errno))
				n_of_bytes_readed = 0;
			// and if it's not interupted maybe it's something else
			else if (!n_of_bytes_readed) {
				infoe_exit("Read error", true, errno);
				return -1;
			}
			// without permision
		} else if (n_of_bytes_readed == -1) {
			infoe_exit("Read error", true, errno);
			return -1;
		}

		/*if message was interupt continue reading the message */
		n_of_bytes_left = n_of_bytes_left - (size_t)n_of_bytes_readed;
		buffer -= n_of_bytes_readed;
	}

	/*if everything is ok we should return the size that has been readed from the fd*/
	return (ssize_t)(len_buffer - n_of_bytes_left);
}

static inline bool is_interupted(ssize_t nbytes, int err)
{
	return ((nbytes < 0) && (err == EINTR)) ? true : false;
}

static ssize_t read_byte_count;
static char *read_ptr;
static char read_buff[MAXLINE];

/**
 * ssize_t char_read
 * Read one char
 * with safe signal handleing
 */
static ssize_t char_read(int fd, char *ptr, bool level)
{
	// check if we have space to read more chars
	if (read_byte_count <= 0) {
	again:
		read_byte_count = read(fd, read_buff, MAXLINE);
		//check for perrmision sign handling
		if (level) {
			if (is_interupted(read_byte_count, errno))
				goto again;
			else if (read_byte_count == -1)
				return -1;
		} else if (read_byte_count == 0) {					  //eof
			return 0;
		} else if (read_byte_count == -1)
			return -1;

		read_ptr = read_buff;
	}

	read_byte_count--;
	*ptr = *read_ptr++;

	return 1;
}

/** 
 * ssize_t sail_readline
 * function reads using his own self buffer types
 * these buffers are static because we need to save them for every
 * execution , if we need them we can simply gem them with a method like void* _get()
 * Note that this function is not treath safety
 */
ssize_t sail_readline(int fd, void *point, size_t len_buffer)
{
	ssize_t n, rc;
	char c, *ptr;

	// pass pointer to char pointer
	ptr = point;

	// for every byte of data
	for (n = 0; n < (int)len_buffer; n++) {
		// if we return successfull a char
		if ((rc = char_read(fd, &c, true)) == 1) {
			// our char pointer point to that char
			*ptr++ = c;
			if (c == '\n')					  // we found a new line
				break;
		} else if (rc == 0) {
			// return the the numebr of bytes that were readed.
			*ptr = 0;					 // make our pointer point to null
			return (n - 1);
		} else
			return -1;
	}

	//point to null
	*ptr = 0;
	//reset read_byte_count if we perform the readline in a while/do/for loop
	//we must reset the byte_count in order to read other lines consecutive from stdin
	read_byte_count = 0;

	return n;
}
