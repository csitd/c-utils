#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h>
#include <unistd.h> 
#include <sys/stat.h> 

/* 
	(Copyright) 2014, MIT license, "posixmkdir.c", csitd 
*/ 

void mkdir_exit(char *, int); 
void drmake(int, char *[], mode_t, int); 

int main (int argc, char *argv[]) 
{ 
	mode_t mode = 0755;
	int o, parents;
	o = parents = 0;
	
	while ((o = getopt (argc, argv, "hpm:")) != -1) 
		switch (o) {
			case 'm': mode = strtol(optarg, NULL, 8); break;
			case 'p': parents = 1; break; 
			case 'h': mkdir_exit("Usage  mkdir -pm:\n", 0); break;
			case '?': mkdir_exit("", 1); break;
			default: break;
		}
	
        argv += optind;
        argc -= optind;

	drmake(argc, argv, mode, parents);

	return 0;

} 

void drmake(int argnum, char *array[], mode_t mode, int parents)
{ 

	int c = 0; 
	size_t len, index;
	char *arrcopy;

        while (c < argnum)
	{ 
		/* handle basic funtionality */
		if ( parents == 0 )
		{
               		mkdir(arrcopy, mode); 
			continue ; 
		}

		/* handle -p functionality  */ 
		len = index = strlen(array[c]);
                arrcopy = array[c++]; 
                for ( ; index > 0 ; --index )
                        if ( arrcopy[index] == '/' )
                                arrcopy[index] = '\0'; 
	
		for ( ; index <= len ; ++index) 
                        if ( arrcopy[index] == '\0' )
			{
				mkdir(arrcopy, mode); 
				arrcopy[index] = '/'; 
			}
        }
} 

void mkdir_exit(char *message, int i){
	printf("%s", message); 
	exit (i);
}

