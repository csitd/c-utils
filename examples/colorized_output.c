#include <stdio.h>


#define ANSI_RESET   "\x1b[0m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_CYAN    "\x1b[36m"


int main() 
{


	printf(ANSI_RED "RED\n" ANSI_RESET);
	printf(ANSI_GREEN "GREEN\n" ANSI_RESET);
	printf(ANSI_YELLOW "YELLOW\n" ANSI_RESET);
	printf(ANSI_BLUE "BLUE\n" ANSI_RESET);
	printf(ANSI_MAGENTA "MAGENTA\n" ANSI_RESET);
	printf(ANSI_CYAN "CYAN\n" ANSI_RESET); 
	return 0;
}
