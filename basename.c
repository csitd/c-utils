#include <stdio.h>
#include <string.h> 

/* (Copyright) 2014, MIT license, "basename.c", csitd */
/* Usage:  basename  [OPTIONS] [PATHS]  */

int main(int argc, char *argv[])
{

	if ( argc != 2 ) return 0;
	size_t len = strlen(argv[1]); 
	unsigned int counter = len;
	while ( argv[1][counter] != '/' )
	{ 
		if ( counter == 0 )
		{
			printf("%s\n", argv[1]);
			return 0;
		}
		--counter;
	}
	while ( counter < len)
        {
		++counter; 
                printf("%c", argv[1][counter]); 
        }
	printf("\n");
	return 0;
		
}

