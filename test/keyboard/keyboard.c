#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
/**
 * Basic implementation of Fgets
 */
#define MAXLINE 255
char * 
Fgets(char *s, int size, FILE *stream)
{
 	char *ret;
 	ret = fgets(s,size,stream);

	if( (ret == NULL) && (ferror(stream))) {
       printf("Fgets error\n");
       exit(EXIT_FAILURE);
	}

	return ret;
}

char buffer[MAXLINE +1];

void read_input (char *s,int size)
{
	Fgets(s,size,stdin);
	int n = (int)strlen(s);
	s[n-1] = '\0';
}

int main(void)
{
	//Fgets(buffer, sizeof(buffer), stdin);
    read_input(buffer,MAXLINE+1);
	for(int i =0; i<sizeof(buffer); i++) {
		if(buffer[i] == '\n')
			printf("buffer[ %d ] = ENTER\n",i);
		else
			if(buffer[i] == '\0')
				printf("buffer[ %d ] = ESCPAE\n",i);
			else 
				printf("buffer[ %d ] = %c\n",i,buffer[i]);
	}
	printf("strlen(buffer) = %lu", strlen(buffer));
}
