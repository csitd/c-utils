#include <stdio.h>

/* (C) 2014 "fibonacci.recursive.c" , C. Graff  */
 
unsigned long fibonacci(int);
 
int main()
{
     int n = 94;
     unsigned long c = 0; 
     
     while ( c < n )
          printf("%lu\n", fibonacci(c++)); 
 
     return 0;
}
 
unsigned long fibonacci(int n)
{ 
     if ( n == 0 || n == 1) 
          return n; 
     return (fibonacci(n - 1) + fibonacci(n - 2));
} 
  
   
 

