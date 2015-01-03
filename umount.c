#include <stdio.h>
#include <sys/mount.h>

/* Free 2014, "umount.c", cgraf */

/* Usage: unmount [PATH(S)] */
/*        umount /mountpoint  */ 


int main(int argc, char *argv[])
{ 
	int counter = 1;
	while ( counter < argc )
		umount(argv[counter++]); 
	return 0; 
} 

