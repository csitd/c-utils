#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h> 

/* C 2014, MIT license, "cat.c" csitd */
/* Usage:     cat [STDIN|FILE(S)] */

void concatenate(int);

int main(int argc, char *argv[])
{ 
	if ( argc == 1 ) concatenate(STDIN_FILENO); 
        while  (*argv) 
		concatenate(open(*++argv,O_RDONLY)); 
	return 0; 
}

void concatenate(int source) 
{
	int n = 0;
	char buf[BUFSIZ];
	while ((n = read(source, buf, BUFSIZ)) > 0) 
		write(STDOUT_FILENO, buf, n);
} 
