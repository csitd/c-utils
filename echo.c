#include <stdio.h> 

/* (Copyright) 2014, MIT license, "echo.c", csitd */

int main(int argc, char *argv[])
{ 
	++argc; // something has to be done with this
	while ( *++argv ) 
	{
		/* Each argument could be parsed at the whitespace
 		   at this point and checked for '\t', '\n' .. etc.
		   Loop over the string at *argv and either execute
		   a literal or escape sequence.
		   If strtok() + strcmp() is used, the string will 
		   have to be expanded.
		  
		*/
		printf("%s", *argv);
		if ( *argv ) 
			printf(" "); 
	} 
	printf("\n");
	return 0;
}
