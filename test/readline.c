#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
FILE *fp;
#define MAXLINE 256
char *line;
int n;
int main(void)
{
    
    fp = fopen("foo.c","r");

    if(fp == NULL){
        printf("Error opening file : %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    for(size_t i = 1; i<(size_t)fp; i++)
    {
        n = read(fileno(fp),line,i);
        if(n<0){
            printf("error read\n");
            exit(EXIT_FAILURE);
        }
        else
            if(n == 0){
                printf("EOF");
                exit(EXIT_SUCCESS);
            }
        printf("%s",line);
    }
    
}
