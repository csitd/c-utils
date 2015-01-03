#include <stdio.h> 
#include <stdlib.h>


int main(int argc, char *argv[])
{ 

	/* Usage:  word_smith [FILE] [WORD(S)] */ 
	FILE *f;
	size_t len = 1000; 
	char *a = malloc(len);
	char *b = malloc(len);
	int c, cntr, cnt = 2; 

	for ( ; cnt < argc ; ++cnt ) { 
		f = fopen(argv[1], "r"); 
		for ( c = 0 ; fscanf(f, "%s", a) != EOF ; cntr = 0 )
			if ( strcmp(argv[cnt], a) == 0 )
                        	for ( ++c ; cntr < len ; )
                                	b[cntr] = a[cntr++]; 
                printf("%s = %d\n", b, c); 	
		fclose(f);
	} 
	return 0;
}

