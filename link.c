#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[])
{ 
	if ((link(argv[1], argv[2])) != 0 || argc != 3)
		fprintf(stderr, "An error has occured\n"); 
	return 0;
} 

