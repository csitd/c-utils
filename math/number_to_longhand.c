#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* (C) 2014, MIT license, number_to_longhand.c , C. Graff */

void num_to_word_err(); 
void num_to_word(char *, int, int); 

int main (int argc, char *argv[])
{
	
	/* check for argument */
	if (!argv[1]) num_to_word_err(); 
	char *array = argv[1]; 
	if (!array) num_to_word_err(); 

	size_t len = strlen(array); 
	int tricount = 1;
	int place = len; 
	int count = 0; 
	
	/* pre calculate any "remainder" that will not fit in a "tricount" */
	int remainder = len % 3;

	/* Now start "lena" at the "remainder" (or 0 if none) */
	unsigned int lena = remainder; 

	/* these arrays get passed to num_to_word() */
	char triple[4] = { 0, 0, 0, 0 };
	char remnant[3] = { 0, 0, 0 }; 
	
	/* handle any partial triplet */
	while ( count < remainder ) 
               remnant[count] = array[count++]; 

	num_to_word(remnant, place, remainder);


	/* handle remaining sets of three */
	while ( lena <= len )
        { 

		/* make the triple to get passed out */
		triple[tricount - 1] = array[lena]; 
		triple[tricount] = '\0' ; 
		
		if ( tricount == 3 )
                      num_to_word(triple, place, tricount); 

		/* reset the "tricount" */
		tricount %= 3;
		for (count = 0; tricount == 3 && count < 4 ;)
                       triple[count++] = 0;
		
                ++lena;
                ++tricount; 
		--place;

        }
	printf("\n");
	return 0;

}

void num_to_word(char *triple, int place, int l)
{ 

 	int a, b, c, d, cnt, count = 0;
	a = b = c = -1; 
	char augged[l]; 
	
	/* hack the string to obtain some integers */
        while ( count < l )
        { 

		for ( cnt = 0; ( augged[cnt] = triple[cnt++] ) ; )
			;
		augged[count + 1] = 0 ; 

		/* Convert chars to ints */ 
		if ( count == 0 ) a = atoi(&augged[count]); 
		if ( count == 1 ) b = atoi(&augged[count]); 
		if ( count == 2 ) c = atoi(&augged[count]); 
		++count; 
	} 

	
	/* adjust place values if triple is less than 3 digits */
	if ( l == 1 ) { c =  a; a = -1; b = -1; }
	if ( l == 2 ) { c =  b; b =  a; a = -1; } 
	
	/* take a, b, c and compute */ 
	
	/* "* hundreds" get handled here */
	if ( a == 1 )  printf("one "); 
	if ( a == 2 )  printf("two "); 
	if ( a == 3 )  printf("three "); 
	if ( a == 4 )  printf("four "); 
        if ( a == 5 )  printf("five "); 
        if ( a == 6 )  printf("six "); 
	if ( a == 7 )  printf("seven"); 
        if ( a == 8 )  printf("eight"); 
        if ( a == 9 )  printf("nine ");  

	/* voice rule for "hundreds" count */ 
	if ( l == 3 && a > 0 )
		printf("hundred ");  
	
	/* all of the "teens" have a special rule */
	if ( b == 1 && c == 0 )  printf("ten ");  
        if ( b == 1 && c == 1 )  printf("eleven "); 
	if ( b == 1 && c == 2 )  printf("twelve "); 
	if ( b == 1 && c == 3 )  printf("thirteen "); 
	if ( b == 1 && c == 4 )  printf("fourteen "); 
	if ( b == 1 && c == 5 )  printf("fifteen ");
	if ( b == 1 && c == 6 )  printf("sixteen "); 
	if ( b == 1 && c == 7 )  printf("seventeen "); 
	if ( b == 1 && c == 8 )  printf("eighteen "); 
	if ( b == 1 && c == 9 )  printf("nineteen "); 

	/* all "*ties" are "tens" places   */
	if ( b == 2 )  printf("twenty "); 
	if ( b == 3 )  printf("thirty "); 
	if ( b == 4 )  printf("fourty "); 
	if ( b == 5 )  printf("fifty "); 
	if ( b == 6 )  printf("sixty "); 
	if ( b == 7 )  printf("seventy "); 
	if ( b == 8 )  printf("eighty "); 
	if ( b == 9 )  printf("ninety ");  

	/* Final columns main rule is to not break the "teens" */
	if ( c == 1 && b != 1 )  printf("one "); 
        if ( c == 2 && b != 1 )  printf("two "); 
        if ( c == 3 && b != 1 )  printf("three ");  
	if ( c == 4 && b != 1 )  printf("four "); 
        if ( c == 5 && b != 1 )  printf("five "); 
        if ( c == 6 && b != 1 )  printf("six ");  
	if ( c == 7 && b != 1 )  printf("seven ");
        if ( c == 8 && b != 1 )  printf("eight "); 
        if ( c == 9 && b != 1 )  printf("nine ");  

	/* Change any unused place values to zero */ 
	d = 0; 
	if ( a < 0 ) a = 0;
	if ( b < 0 ) a = 0;
	if ( c < 0 ) a = 0;
	d = a + b + c ; 
	
	/* places */
	if ( place < 7  && place > 3  && d > -1 )  printf("thousand ");
        if ( place < 10 && place > 6  && d > -1 )  printf("million ");
	if ( place < 13 && place > 9  && d > -1 )  printf("billion ");
	if ( place < 16 && place > 12 && d > -1 )  printf("trillion ");
	if ( place < 19 && place > 15 && d > -1 )  printf("quadrillion ");
	if ( place < 22 && place > 18 && d > -1 )  printf("quintillion ");
	if ( place < 25 && place > 21 && d > -1 )  printf("sextillion ");
	if ( place < 28 && place > 24 && d > -1 )  printf("septillion "); 

	/* reset all the place holder variables */
	a = b = c = -1;

} 


void num_to_word_err(){
        fprintf (stderr, "Argument error.\n");
        exit (1);
} 


