#include <stdio.h>
#include <stdlib.h>

int error_check(int flag)
{
	if (flag)
	{
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	
	int flag = 1;


	error_check(flag);

	printf("Totul e ok\n");

	return 0;
}