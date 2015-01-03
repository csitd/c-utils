#include <stdio.h> 
//#include <stdlib.h>


/* (C) 2014, MIT license, "selection_sort.c" , C. Graff */


void print_array(unsigned long *, int);
void exchange(unsigned long *, unsigned long *); 
void selection_sort(unsigned long *, unsigned long); 
 
int main()
{ 
    
	unsigned long c = 0;
	unsigned long elements = 100000;
	unsigned long *numray = malloc(elements * sizeof(unsigned long));

	srandom(time(NULL));
	for (; c < elements ; c++)
		numray[c] = (random () % 9999999999) * 1000000000000000000; 
  
	print_array(numray, elements);
	printf("====================\n");    
	selection_sort(numray, elements); 
	print_array(numray, elements);

	return 0;
}

void selection_sort(unsigned long *numray, unsigned long n)
{ 

	unsigned long posit, c, d; 

	for ( c = 0 ; c < ( n - 1 ) ; c++ ) { 
		posit = c; 
		for ( d = c + 1 ; d < n ; d++ ) 
			if ( numray[posit] > numray[d] )
				posit = d;
	    
		if ( posit != c )
			exchange(&numray[c], &numray[posit]); 
	    
	} 

}

void exchange(unsigned long *a, unsigned long *b)
{
	unsigned long cache = *a; 
	*a = *b;
	*b = cache;
}


void print_array(unsigned long *a, int b)
{
	int c;		    
	for (c=0 ; c < b ; c++)
		printf("%lu\n", a[c]); 
}


