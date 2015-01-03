#include <stdio.h>

/* (Copyright) 2014, MIT license, "mv.c", csitd */

int main(int argc, char *argv[])
{
	if ( argc != 3 ) 
		return 0; 
	rename(argv[1], argv[2]);

	return 0; 
}
