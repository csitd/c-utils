#include <stdio.h>

void exchange(unsigned long *, unsigned long *);

int main()
{
	unsigned long array[2] = { 99999, 88888 };
	printf ("Zeroth element is %lu first is %lu \n",array[0], array[1]); 
	exchange(&array[0], &array[1]);
	printf ("Zeroth element is %lu first is %lu \n",array[0], array[1]);
}

void exchange(unsigned long *a, unsigned long *b)
{
        unsigned long cache = *a;
        *a = *b;
        *b = cache;
}

