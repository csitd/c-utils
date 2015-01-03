#include <stdio.h> 

/* (C) 2014, MIT license, "convert_case.c", conver_case.c  */


int main ( int argc, char *argv[] )
{
	int c, o, af, uf; 
	while ((o = getopt (argc, argv, "ulh")) != -1) switch (o) {
		case 'l': af = 1; break;
		case 'u': uf = 1; break;
		case 'h': printf("Usage: STREAM | convert_case [-u|-l]\n"); return 1;
		case '?': return 1;
	}
	while ((c = getchar()) != EOF)
	{
		if ((af) == 1 )
			putchar(tolower(c)); 
		else if ((uf) == 1 )
			putchar(toupper(c));
	}

	return 0;
}


