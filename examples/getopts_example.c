#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>


void error(); 

int main (int argc, char *argv[])
{
	int counter = 0;
	char o;

	while ((o = getopt (argc, argv, "ab:")) != -1)
	{ 
		switch (o) {
			case 'a':
				printf ("Option -a activated\n");
				break;
			case 'b':
				printf ("Option -b activated with argument: %s.\n", optarg);
				break; 
			default: error();
		
			
		}
	} 

	argv += optind; 
	argc -= optind;
 
   
        while ( counter < argc )
            printf ("Remaining arg:  %s\n", argv[counter++]);
       

    return 0;
}


void error(){
        fprintf (stderr, "Program has encounterd an error.\n");
        exit (1);
}
