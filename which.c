#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h> 
#include <unistd.h>


/* (Copyright) 2014, MIT License, "which.c", csitd */


int main (int argc, char *argv[])
{ 

	struct stat sb;
	int argcount = 1;
	int cnt, o, all = 0;
	unsigned int count, counter;

	


	while ((o = getopt (argc, argv, "ah")) != -1)
                switch (o) { 
                        case 'a': all = 1; continue; 
			case 'h': printf("Usage: which [OPTION] [FILE(S)]\n");
				  printf(" -a\tPrint all matches\n");
				  printf(" -h\tPrint this menu and exit\n");
				  exit (0); 
                        case '?': exit (1);
                } 


	for ( ; argcount < argc ; ++argcount )
	{ 

		if (!argv[argcount]) exit(0);

		cnt = count = counter = 0;
		size_t b = strlen(argv[argcount]);
		size_t a = strlen(getenv("PATH")); 

		if ( a > 10000 ) exit(1);

		char *word = malloc(a + b + 1); 
		char *argg = malloc(b);
		argg = argv[argcount]; 

		if (!argg || !word) exit(1); 

		for ( ; count <= a ; ++count )
		{

			if ( getenv("PATH")[count] != ':' ) 
				word[cnt] = getenv("PATH")[count]; 

			/* zero out and increment */
			word[++cnt] = '\0';

			if ( getenv("PATH")[count] == ':' || count == a )
			{ 
				
				if ( word[--cnt] != '/' )
                                        word[cnt++] = '/'; 
		
				while (counter < b) 
					word[cnt++] = argg[counter++]; 
		
				word[cnt] = '\0';
				if ( stat(word, &sb) == 0 && sb.st_mode & S_IXUSR )
				{ 
					printf("%s\n", word); 
					if ( all != 1 )
						break;
				} 	
               			cnt = counter = 0; 
			} 
		} 
	}
	return(0); 
}

