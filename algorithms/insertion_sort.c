#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>



/* (C) 2014, MIT license, "insertion_sort.c" , C. Graff */

void print_array(unsigned long *, unsigned long); 
void insertion_sort(unsigned long *, unsigned long); 
void exchange(unsigned long *, unsigned long *);

 
int main()
{
	unsigned long c = 0;
	unsigned long elements = 10000;
	unsigned long *numray = malloc(elements * sizeof(unsigned long));

	srandom(time(NULL));
	for (; c < elements ; c++)
		numray[c] = (random () % 9999999999) * 1000000000000000000; 

	print_array(numray, elements);
	printf("====================\n");
	insertion_sort(numray, elements);  
	print_array(numray, elements); 

	return 0; 
}

void insertion_sort(unsigned long *numray, unsigned long n)
{ 
	unsigned long c, d; 
	for ( c = 1; c < n ; c++ ) 
		for ( d = c; d > 0 && numray[d] < numray[ d - 1 ] ; d-- )
			exchange(&numray[d], &numray[ d - 1 ]); 
}

void exchange(unsigned long *a, unsigned long *b)
{
	unsigned long cache = *a; 
	*a = *b;
	*b = cache; 
} 


void print_array(unsigned long *a, unsigned long b)
{
	unsigned long c;
	for (c=0 ; c < b ; c++)
		printf("%lu\n", a[c]);

} 

