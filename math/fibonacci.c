#include <stdio.h> 
#include <stdlib.h> 


/* C 2014, MIT license, "fibonacci.c" C. Graff */

/* Populate an array with the fibonacci number sequence. */

unsigned long fibonacci(unsigned long); 

void print_array(unsigned long *, unsigned long); 
 
int main()
{ 

     unsigned long c = 0;
    
     /* after "94" iterations the fibonacci number is too large for an unsigned int */
     unsigned long elements = 94; 
     unsigned long *numray = malloc(elements * sizeof(unsigned long)); 


     while (c < elements) 
          numray[c++] = fibonacci(c + 1); 

     print_array(numray, elements);

     return 0;
}

void print_array(unsigned long *a, unsigned long b)
{

     unsigned long c;              
     for (c=0 ; c < b ; c++)
         printf("%lu\n", a[c]); 
}

unsigned long fibonacci(unsigned long n)
{ 

     unsigned long alpha=0, omega=1, incra=0, c=0; 

     for ( ; c < n ; c++ ) { 
          if ( c <= 1 ) 
               { incra = c; continue; } 
          incra = alpha + omega;
          alpha = omega;
          omega = incra; 
     } 

     return (incra);

} 



