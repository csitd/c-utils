#include <stdio.h> 

/* "seconds_to_minutes.c", */

int main(int argc, char *argv[]) 
{ 
	if (!argv[1]) return 1; 

	int a = atoi(argv[1]); 
	printf("seconds : %d\n", a % 60); 
	printf("minutes : %d\n", a / 60); 
	return 0; 
}
