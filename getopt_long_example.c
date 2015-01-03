#include <stdio.h> 
#include <getopt.h> 
#include <stdlib.h>


void error(); 

int main (int argc, char *argv[])
{
	int c;
	int counter = 0;
	int option_index = 0; /* important */

	static struct option long_options[] =
	{ 
		{"name",  required_argument, 0, 'a'}, 
		{"b",  required_argument, 0, 'b'},
		{0, 0, 0, 0}
	}; 

	while (( c = getopt_long_only (argc, argv, "a:b:",
			 long_options, &option_index)) != -1 ) 
		switch (c) { 
			case 'a': printf("-name %s\n", optarg); break; 
			case 'b': printf("-b %s\n", optarg); break;
			case '?': error(); 
		} 

	argv += optind;
        argc -= optind; 

        while ( counter < argc )
		printf ("Remaining arg:  %s\n", argv[counter++]); 

	return 0; 

}


void error(){
        fprintf (stderr, "This is an error function.\n");
        exit (1);
}


