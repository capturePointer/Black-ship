#include <stdio.h>

int main(int argc, char *argv[]) {
        printf("a char is %ld bytes , %ld bits\n", sizeof(char),sizeof(char)*8);
        printf("an int is %ld bytes, %ld bits\n", sizeof(int),sizeof(int)*8);
        printf("an float is %ld bytes, %ld bits\n", sizeof(float),sizeof(float)*8);
        printf("a double is %ld bytes, %ld bits\n", sizeof(double),sizeof(double)*8);
        printf("a short int is %ld bytes, %ld bits\n", sizeof(short int),sizeof(short int)*8);
        printf("a long int is %ld bytes, %ld bits\n", sizeof(long int),sizeof(short int)*8);
        printf("a long double is %ld bytes, %ld bits\n", sizeof(long double),sizeof(long double)*8);
        return 0;
}
