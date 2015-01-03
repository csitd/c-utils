#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* (Copyright) 2014, MIT license, "brute.c", C. Graff 

   Usage:   brute [LETTER(S)] [DEPTHSTART] [DEPTHEND]	

	~$ ./brute abc 1 3 
		a
		b
		c
		aa
		ab
		ac
		ba
		bb
		bc
		ca
		cb
		cc
		aaa
		aab
		aac ....

*/


int truth_table(char , char *);
void abrute(char *, char *, int, int); 

void has_happened(char *, char *);
void still_repeats(char , char *);


int main(int argc, char *argv[]) 
{ 
	if ( argc < 4 ) return 0;

	int len = atoi(argv[2]);
	int end = atoi(argv[3]); 
	char a[len];
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
			if ( index < length - 1 ) 
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

void has_happened(char *strhold, char *event)
{
	int c = 0; 
	int d = 0;
	event = malloc(1000); 
	
	while ( strhold[c] != '\0' )
	{

		event[d++] = strhold[c++];
		event[c + 1] = '\0' ;

	} 

} 

