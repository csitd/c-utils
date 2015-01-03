#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <netdb.h> 
#include <fnmatch.h>

/* Needed for freebsd libc */
#include <sys/types.h>
#include <sys/socket.h>

/* (Copyright) 2014, MIT license, "fetch.c", csitd */

void fetch_err(char *); 
void fetch_page(char *); 

int main (int argc, char *argv[])
{ 
	/* Usage:  fetch  http://www.gnu.org/  */ 
	int count = argc;
	++count;
	++argv;
	while (*argv)
		fetch_page(*argv++);
	return 0;
}

void fetch_page(char *hold)
{ 
	struct addrinfo hints, *res; 
        memset(&hints, 0, sizeof(hints)); 

	size_t buflen = 10001; 
	char buf[buflen];
	memset(buf, 0, buflen);

        unsigned int sck, c, cnt, l, nln, reset, bcnt, i;
	c = cnt = l = nln = reset = bcnt = i = 0; 

	/* Process argument */ 
	size_t len = strlen(hold) + 1; 
	size_t leng = -5; 
	char message[len + 40];
	char host[len];
	char type[len];
	char page[len]; 
       
        for ( ; i < len ; ++i ) 
		type[i] = host[i] = page[i] = '\0'; 
	
	for ( i = 0 ; hold[c] != ':' && c < len ; ) 
		type[i++] = hold[c++]; 
	for ( c += 3 ; hold[c] != '/' && c < len ; ) 
		host[cnt++] = hold[c++]; 
	for ( c += 1 ,cnt = 0 ; hold[c] != '\0' && c < len ; ) 
                page[cnt++] = hold[c++]; 

	/* Put all the information together into a "message" */ 
	sprintf(message, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n", page, host); 

	/* Unspecified connection type ??  */
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; 
	if ( ( getaddrinfo(host, type, &hints, &res) ) != 0 ) 
		fetch_err(" getaddrinfo(2) failed"); 

	/* assign fd of the socket to "sck" */ 
	sck = socket(res->ai_family, res->ai_socktype, res->ai_protocol); 
	
	if ( (connect(sck, res->ai_addr, res->ai_addrlen) ) == -1 )
		fetch_err(" connect(2) failed"); 
  
	send(sck, message, strlen(message), 0); 


	/* Start recieving 'recv()' the transmission */
	while ( leng != 0 )
	{ 	
		leng = recv(sck, buf, buflen, 0);
		while ( bcnt < leng ) 
		{ 
			/* Filter off the HTTP header */ 
			if ( l == 0 && buf[bcnt] >= '!' ) nln = 0; 
			if ( l == 0 && buf[bcnt] == '\n' ) ++nln; 
                        for (; nln == 2 && l == 0 && buf[bcnt] == '\n'; l = 1)
                                ++bcnt; 
			/* Continue with the rest of the file */
			if ( l == 1 ) 
				printf("%c", buf[bcnt]); 
	
                        ++bcnt; 
		} 	
	
		bcnt = reset = 0; 
		while ( reset < buflen ) 
			buf[reset++] = '\0'; 
	}
	freeaddrinfo(res);
	close(sck); 
}

void fetch_err(char *report){
	fprintf (stderr, "%s\n", report);
        fprintf (stderr, "Please specify a valid http or ftp address.\n");
        exit (1);
} 

