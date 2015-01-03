#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h> 

/* (Copyright) 2014, MIT license, "grep.c", csitd */ 

void grep_error(char *); 

int main(int argc, char **argv)
{ 

	FILE *fp;
	char line[BUFSIZ];
	regex_t re;
	regmatch_t rm[20];
        int o, ofunc = 0, c = 1;
	
	if ( argc < 2 ) 
		grep_error("Usage: grep [OPTIONS] [PATTERN] [FILE(S)].. \n");

        while ((o = getopt (argc, argv, "o")) != -1)
                switch (o) {
                        case 'o': ofunc = 1; continue;
                        case '?': grep_error("Invalid option\n");
                } 
        argv += optind;
        argc -= optind; 

	if((regcomp(&re, argv[0],  REG_EXTENDED)) != 0)
		grep_error("Regex Failed!\n"); 
	
	while ( c - argc || argc == 1)
	{
		if (argc == 1 ) fp = stdin;
		else fp = fopen(argv[c], "r");

		while ((fgets(line, BUFSIZ, fp)) != NULL)
		{
        		line[strlen(line)-1] = '\0';
			if (!((regexec(&re, line, 2, rm, 0)) == 0))
				continue;
			if (ofunc != 1)
			{
		
				printf("%s\n", line);
				continue;
			} 
	
		}
		++c;
		if (argc == 1 ) return 0;
	}
	regfree(&re);
	return 0;
}



void grep_error(char *message){
     fprintf (stderr, "%s", message);
     exit (1);
}

