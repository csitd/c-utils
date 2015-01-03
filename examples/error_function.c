#include <stdio.h>
#include <stdlib.h> 

void local_error(char *);

int main() {
	local_error("This is an error message"); 
}

void local_error(char *errmess){
	fprintf (stderr, "%s", errmess); 
	fprintf (stderr, "This is an error function.\n");
	exit (1); 
}







