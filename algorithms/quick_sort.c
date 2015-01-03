#include <stdio.h> 
#include <stdlib.h>



/* (C) 2014, MIT license, "quick_sort.c" , C. Graff */ 


void quick_sort(unsigned long *, int, int);
void print_array(unsigned long *, int);
void exchange(unsigned long *, unsigned long *);
 
int main()
{ 

	unsigned long c = 0;
	unsigned long elements = 10000; 
	//int ret;
	unsigned long *numray = malloc(elements * sizeof(unsigned long)); 
	srandom(time(NULL)); 
	for (; c < elements ; c++) 
		numray[c] = (random () % 9999999999) * 1000000000000000000; 
		// numray[c] = (random () % 9) * 1;
   
	/* pass the array to "quick_sort" */
	print_array(numray, elements); 
	printf("====================\n");
	quick_sort(numray, 0, elements - 1); 
	print_array(numray, elements); 

} 

void exchange(unsigned long *a, unsigned long *b)
{
	unsigned long cache = *a; 
	*a = *b;
	*b = cache;
}



void quick_sort(unsigned long *numray, int alpha, int omega)
{
   
	unsigned long key, left, right, pivot; 

	if (alpha < omega)
	{ 
		/* calculate partition offset */
		pivot = ((alpha + omega) / 2); 
               
		exchange(&numray[alpha],&numray[pivot]); 

		key = numray[alpha];
		left = alpha + 1;
		right = omega;
		while (left <= right)
		{
			while ((left <= omega) && (numray[left] <= key))
				left++;
			while ((right >= alpha) && (numray[right] > key))
				right--;
			if (left < right)
				exchange(&numray[left], &numray[right]); 
		}
		/* exchange elements */ 
		exchange(&numray[alpha], &numray[right]); 
         
       
		quick_sort(numray, alpha, right - 1);
		quick_sort(numray, right + 1, omega);
	}
}
void print_array(unsigned long *a, int b)
{
	int c; 
	for (c=0 ; c < b ; c++) 
		printf("%lu\n", a[c]); 

}


