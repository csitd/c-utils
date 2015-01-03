#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 

/* 
   (Copyright) 2014, MIT license, "wc.c" csitd 

   Usage:     wc [STDIN|FILE(S)] 
   
   cc wc.c -Wall -Wextra -o wc

   Notes: 
   o Matches word count of freebsd's wc.c, but not 
     GNU's, or busybox's.  
   o There is no -m option implemented.
*/

struct cnt{ 
	int l;
	int w;
	int c;
}cnt; 

void word_count(FILE *, int, int, int);
void wc_help(int);
void wc_print(int, int, int, int, int, int);

int main ( int argc, char *argv[] )
{

	FILE *fip; 
	int o, l, w, c;
	int i, j, k;
	o = l = w = c =1;
	i = j = k = cnt.l = cnt.w = cnt.c = 0; 

	while ((o = getopt (argc, argv, "lwch")) != -1)
                switch (o) { 
                       	case 'l': j = 1; continue;
			case 'w': k = 1; continue; 
			case 'c': i = 1; continue;
			case 'h': wc_help(0); 
			case '?': wc_help(1);
                }
	if ( i == 1 && j == 0 && k == 0 )
		l = w = 0;
	if ( i == 0 && j == 1 && k == 0 )
		c = w = 0;
	if ( i == 0 && j == 0 && k == 1 )
		c = l = 0;
	if ( i == 1 && j == 1 && k == 0 )
		w = 0;
	if ( i == 1 && j == 0 && k == 1 )
		l = 0;
	if ( i == 0 && j == 1 && k == 1 )
                c = 0; 

        argv += optind;
        argc -= optind;

	if ( argc > 0 )  
	{ 
		while (*argv)
		{ 
			if ((fip = fopen(*argv , "rb")))
                        { 
				word_count(fip, l, w, c); 
				printf("%s\n", *argv); 
				fclose(fip); 
			} 
			++argv;
		}
		if ( argc > 1 )
		{
			wc_print(l, w, c, cnt.l, cnt.w, cnt.c); 
			printf("total\n"); 
		}
	}
	else { 
		word_count(stdin, l, w, c);
		printf("\n");
	} 
	return 0;
}

void word_count(FILE *source, int l, int w, int c)
{
	int o, nl, nw, nc, state;
	o = nl = nw = nc = state = 0;

	while ( (o=fgetc(source)) != EOF )
        { 
		switch(o){
			case '\n' : state = 0;
                        	++nl;
				++cnt.l;
				break;
			case '\r' : state = 0;
				break;
			case '\f' : state = 0;
                                break;
			case ' '  : state = 0;
                                break; 
			case '\t' : state = 0;
                                break;
			case '\v' : state = 0;
                                break;
			default :  
				if (state == 0)
				{
					++nw;
					++cnt.w;
                        		state = 1;
				}
		}
		++nc; 
		++cnt.c;
        } 
	wc_print(l, w, c, nl, nw, nc);

}

void wc_help(int err)
{
	fprintf(stderr,"-h help\n-l lines\n-w words\n-c chars\n");
        exit (err);
}

void wc_print(int l, int w, int c, int nl, int nw, int nc)
{
	if ( l == 1 ) printf("%6d  ", nl);
        if ( w == 1 ) printf("%6d  ", nw);
        if ( c == 1 ) printf("%6d  ", nc);
}

