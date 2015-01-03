#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h>
#include <unistd.h> 
#include <sys/stat.h> 

/* 
	(Copyright) 2014, MIT license, "mkdir.c", csitd 
*/ 

void mkdir_exit(char *, int); 
void mkdir_verbose(char *, mode_t, int); 
void drmake(int, char *[], mode_t, int, int); 


int main (int argc, char *argv[]) 
{ 

	mode_t mode = 0755;
	int o, parents, verbose;
	o = parents = verbose = 0;
	
	while ((o = getopt (argc, argv, "hvpm:")) != -1) 
		switch (o) {
			case 'm': mode = strtol(optarg, NULL, 8); break;
			case 'p': parents = 1; break;
			case 'v': verbose = 1; break;
			case 'h': mkdir_exit("Usage  mkdir -pvm:\n", 0); break;
			case '?': mkdir_exit("", 1); break;
			default: break;
		}
	
        argv += optind;
        argc -= optind;

	drmake(argc, argv, mode, verbose, parents);

	return 0;

} 

void drmake(int argnum, char *array[], mode_t mode, int verbose, int parents)
{ 

	int c = 0; 
	size_t len, index;
	char *arrcopy;

        while (c < argnum)
	{

                len = index = strlen(array[c]); 
		
                arrcopy = array[c++]; 
		
		/* handle basic funtionality */
		if ( parents == 0 )
		{
               		mkdir(arrcopy, mode);
                	mkdir_verbose(arrcopy, mode, verbose); 
			continue ; 
		}

		/* handle -p functionality  */
		/* Strip delimeters */
                for ( ; index > 0 ; --index )
                        if ( arrcopy[index] == '/' )
                                arrcopy[index] = '\0';

		/* Add delimeters back one at a time */
		for ( ; index <= len ; ++index) 
                        if ( arrcopy[index] == '\0' )
			{
				mkdir(arrcopy, mode); 
				mkdir_verbose(arrcopy, mode, verbose); 	
				arrcopy[index] = '/'; 
			}
        }
}

void mkdir_verbose(char *arrcopy, mode_t mode, int verbose){
	if ( verbose == 1 ) 
		printf("Making directory: %s  mode: %o\n", arrcopy, mode); 
}

void mkdir_exit(char *message, int i){
	printf("%s", message); 
	exit (1);
} 

