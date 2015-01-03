#include <stdio.h> 
#include <stdlib.h>



/* (C) 2014, MIT license, "bubble_sort.c" , C. Graff */

void print_array(unsigned long *, int);
void exchange(unsigned long *, unsigned long *); 
void bubble_sort(unsigned long *, int);

 
int main(int argc, char *argv[])
{ 
    
	unsigned long c = 0; 
	unsigned long elements = 10000; 
	unsigned long *numray = malloc(elements * sizeof(unsigned long));

	srandom(time(NULL));
	for (; c < elements ; c++)
		numray[c] = (random () % 9999999999) * 1000000000000000000; 
    
	print_array(numray, elements);
	printf("====================\n");    
	bubble_sort(numray, elements); 
	print_array(numray, elements);

	return 0;
}

void bubble_sort(unsigned long *numray, int n)
{ 
   
	unsigned long varhold, c, d;

	for (c = 0 ; c < ( n - 1 ); c++) {
		for (d = 0 ; d < n - c - 1; d++) {
			if (numray[d] > numray[d+1]) {
				exchange(&numray[d], &numray[d+1]); 
			}
		}
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


