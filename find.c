#include <stdio.h>
#include <dirent.h> 
#include <fnmatch.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h> 


/* 
	(Copyright) 2014, MIT license, "find", csitd 
*/ 

	
struct states {
	char cond[1024][1024];
	int truth[1024];
	int or;
	int n;
} states;

void find_pattern(char *);

int main(int argc, char *argv[])
{ 
	int c; 
        int option_index = 0; /* important */
	size_t len = 1024; 
	
        static struct option long_options[] =
        {
                {"name",  required_argument, 0, 'a'},
              //  {"o",  required_argument, 0, 'b'},
                {0, 0, 0, 0}
        };

	states.n = 0;

        while (( c = getopt_long_only (argc, argv, "a:b:o",
                         long_options, &option_index)) != -1 )
                switch (c) {
                        case 'a': memset(states.cond[states.n], 0, len); 
				  strcpy(states.cond[states.n], optarg); 
				  if ( states.or == 1 )
					states.truth[states.n] = 1;
				  else 
					states.truth[states.n] = 0; 
				  ++states.n; 
				  states.or = 0;
				  break;
                        case 'b': printf("-b %s\n", optarg); break;
			case 'o': states.or = 1 ; break;
                        case '?': break;
			default: break;
                }

        argv += optind;
        argc -= optind; 


	find_pattern(argv[0]); 

	return 0;
} 

void find_pattern(char *path)
{ 
	DIR *dir;
	struct dirent *dentry; 
	size_t len = 10000;
	char spath[len]; 
	memset(spath, 0, len); 
	int p = 0;
	int good = 0;

	size_t plen = strlen(path);
	if ( path[plen - 1] == '/' && plen > 1 ) 
			path[plen - 1] = '\0'; 	
	
	if ( ( dir = opendir(path) ) ) 
	{
		dentry = readdir(dir); 
		while ( dentry )
		{ 
			if ( len > 1 ) 
				sprintf(spath, "%s/%s", path, dentry->d_name);
			else 
				sprintf(spath, "%s%s", path, dentry->d_name);

			p = 0;
			good = 0;
			while ( p < states.n )
			{ 
				printf("Seq: %d\n", p);
				printf("FILE  %s\n", dentry->d_name);
				if ( fnmatch(states.cond[p], dentry->d_name, FNM_PERIOD) == 0  ) 
				{
				
					printf("match %s\n", states.cond[p]);
					printf("Truth state is: %d\n", states.truth[p]);
					
					good = 1;
						
                               

				} else 
				{
					printf("not match %s\n", states.cond[p]);
		
                                         ;
				
                                  
				}
				//if ( states.truth[p] == 1 )
				++p;
			}
			
			if ( good > 1  ) 
				printf( "%s\n", spath); 

			if ( dentry->d_type == DT_DIR &&
			   ( strcmp( ".", dentry->d_name)) &&
			   ( strcmp( "..", dentry->d_name))) 
				find_pattern(spath); 

			dentry = readdir(dir); 
		}
	}
	closedir(dir); 
}



