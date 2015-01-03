#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* (Copyright) 2014, MIT license, "simple_brute.c", C. Graff

   Usage:  simple_brute [range start] [range end] 
            example:      ./simple_brute 1 3 

*/

void brute(char *, int, int);

int main(int argc, char *argv[]) 
{ 

	if (argc < 3 ) return 1;

	int depth = atoi(argv[2]);

	int len = atoi(argv[1]); 

	while ( len <= depth )
        	brute(argv[1], 0, len++);
	return 0;
}


void brute(char *string, int index, int length)
{ 
	char cache = '!' ; 

	while ( cache <= '~' ) {
		string[index] = cache; 
		if (index < (length - 1 )) 
			brute(string, index + 1, length); 
		else {
			string[index + 1] = '\0';
			printf("%s\n", string); 	
		}
		++cache; 
	} 
} 

