#include <stdio.h> 
#include <stdlib.h>



/* (C) 2014, MIT license, "pascal_farleft.c" , C. Graff */ 

/* calculate the inner most side of pascals triangle */



void print_array(unsigned long *, unsigned long);

unsigned long pascal_farleft(unsigned long *, unsigned long, int);

 
int main()
{ 
    
	/* set number of array elements to extrapolate to */
	unsigned long elements = 1000000;

	/* set number of inner iterations */
	int max = 4;
     
	unsigned long *numray = malloc(elements * sizeof(unsigned long)); 

	pascal_farleft(numray, elements, max);
	print_array(numray, elements);

	return 0;
}

void print_array(unsigned long *a, unsigned long b)
{
	unsigned long c;              
	for (c=0 ; c < b ; c++)
		printf("%lu\n", a[c]); 
}

unsigned long pascal_farleft(unsigned long *b, unsigned long n, int max)
{

	unsigned long c, i, k; 
	unsigned long cnt, hold, inner; 
	cnt = hold = inner = 0;

    
	for (i = 0 ; i < n ; i++)
	{
		hold = inner = 0;
		c = k = 1; 
		for (; k <= i && cnt < n && c >= hold ; k++) 
		{ 
			b[cnt++] = c ; 
			hold = c; 
			c = c * ( i - k ) / k ; 
			++inner;
			if ( inner >= max )
				break ;

		} 
	}
	return 0;
}


