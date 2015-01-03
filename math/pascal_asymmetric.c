#include <stdio.h> 
#include <stdlib.h>



/* (C) 2014, MIT license, "pascal_asymmetric.c" , C. Graff */ 


void print_array(unsigned long *, unsigned long);

unsigned long pascal_asymmetric(unsigned long *, unsigned long);

 
int main(int argc, char *argv[])
{ 
    
	/* any higher than "1051" overflows the unsigned long */
	unsigned long elements = 1051;
	unsigned long *numray = malloc(elements * sizeof(unsigned long)); 

	pascal_asymmetric(numray, elements);
	print_array(numray, elements);

	return 0;
}

void print_array(unsigned long *a, unsigned long b)
{
	unsigned long c;              
	for (c=0 ; c < b ; c++)
		printf("%lu\n", a[c]); 
}

unsigned long pascal_asymmetric(unsigned long *b, unsigned long n)
{ 

	unsigned long hold, i, k, c; 
	unsigned long cnt = 0; 
    
	for (i = 0 ; i < n ; i++)
	{
		hold = 0;
		c = k = 1; 
		for (; k <= i && cnt < n && c >= hold ; k++) 
		{ 
			b[cnt++] = c ; 
			hold = c; 
			c = c * ( i - k ) / k ; 
		} 
	}
	return 0;
}




