#include <stdio.h> 
#include <time.h>
#include <string.h> 
#include <stdlib.h> 

/* (Copyright) 2014, MIT license, "sleep.c", csitd */ 

void sleep_err(char *);
int main(int argc, char *argv[])
{ 
		
	struct timespec timea, timeb;
	int i = 1;
	size_t len;
	double j;
	long k;

	if (!argv[1]) 
		sleep_err("Usage:  sleep [NUMBERs|m|h|d]\n");

	while ( i < argc ) 
	{ 
		len = strlen(argv[i]);
		j = ( strtod(argv[i], NULL) * 1000000000 );
		if ( argv[i][len-1] == 'm' ) 
			j *= 60;
		if ( argv[i][len-1] == 'h' ) 
			j *= 60*60;
		if ( argv[i][len-1] == 'd' ) 
			j *= 60*60*24;
		k = j;
		timea.tv_sec = (k / 1000000000);
		timea.tv_nsec = (k % 1000000000);
		nanosleep(&timea , &timeb);
		++i;
	}
	return 0;
} 

void sleep_err(char *errmess){
        fprintf (stderr, "%s", errmess);
        exit (1);
}
