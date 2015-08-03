#include <stdio.h>
#include <stdlib.h>


int main(void)
{


	int ret;

	ret = system("./printenv");
	perror("system");
	printf("ret = %d\n",ret);
	return 0;
}
