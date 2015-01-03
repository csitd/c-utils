#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int truth_table(char , char *);
void abrute(char *, char *, int, int); 


int main(int argc, char *argv[]) 
{ 

	/* Usage:   brute [LETTER(S)] [DEPTHSTART] [DEPTHEND]*/
	if ( argc < 4 ) return 0;
	int len = atoi(argv[2]);
	int end = atoi(argv[3]); 
	char a[end]; 
	int b = 0;

	while ( b <= end ) 
		a[b++] = '\0'; 
	
	while ( len <= end )
        	abrute(a, argv[1], 0, len++);

	return 0;
}


void abrute(char *string, char *strhld, int index, int length)
{ 
	char cache = '!' ; 
	while ( cache <= '~' ) { 
		string[index] = cache; 
		if ( truth_table(cache, strhld) == 1 ) 
		{
			if ( index < length - 1  ) 
				abrute(string, strhld, index + 1, length);
			else
				printf("%s\n", string); 
		}
		++cache; 
	} 
}

int truth_table(char cache, char *strhld)
{ 
	int count = 0; 
	while ( strhld[count] != '\0' ) 
		if ( cache == strhld[count++] ) 
			return 1; 
	return 0; 
}
