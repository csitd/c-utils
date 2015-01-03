#include <stdio.h>

int main()
{
	char a = 0; 

	while( a <= '~' ) 
	{
		printf("%d %c \n", a, a);
		++a;
	}
	return 0; 
}
