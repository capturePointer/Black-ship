#include <stdio.h>
#include <stdlib.h>

struct ListItem
{
	/*
	 * what is stored in our node 
	 */
	int data;
	struct ListItem *next;
};



int 
main(void)
{

	struct ListItem a;

	/*our first node*/
	a.data = 2;
	a.next = NULL;


	struct ListItem b;

	b.data = 4;
	b.next = NULL;

	struct ListItem c;
	c.data = 6;
	c.next = NULL;

	a.next = &b;
	b.next = &c;
	
	int cnt = 0;


	for( struct ListItem *i = &a; i!=NULL; i = i->next)
	{
		printf("Item %d value is %d\n" , cnt , i->data);
		cnt++;
	}

}

