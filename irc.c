#include <stdio.h>
#include <string.h> 
#include <netdb.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/mman.h> 
#include <signal.h> 
#include <time.h> 
/* FreeBSD */ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 

/* 
   (Copyright) 2014, MIT license, "irc.c", csitd 
*/

struct colors {
	char *res; 
	char *yel; 
	char *red; 
	char *blu; 
	char *mag; 
	char *gre; 
	char *cya;
} colors = { 
	"\x1b[0m",
	"\x1b[33m", 
	"\x1b[31m", 
	"\x1b[34m", 
	"\x1b[35m", 
	"\x1b[32m", 
	"\x1b[36m" 
};


struct sel{
	char *channel; 
	int *off;
} sel;

void print_time();
void irc_help(); 
void detect_message(char *);
void process_in(char *, size_t);
void irc_client(char *name, char *server, int port);

int main(int argc, char **argv) 
{

	size_t len = 8190; 
	char *server, *name; 
	int o, port = 0; 
	
	sel.channel = mmap(NULL, sizeof *sel.channel, 
		PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sel.off = mmap(NULL, sizeof (int*), 
		PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);    
	sel.off[1] = 0;
	memset(sel.channel, 0 , sizeof(struct sel));
        while ((o = getopt (argc, argv, "hs:n:c:p:")) != -1)
                switch (o) {
                        case 's': server = optarg; continue;
                        case 'n': name = optarg; continue;
                        case 'c': memset(sel.channel, 0, len);
				  strcpy(sel.channel, optarg); continue;
                        case 'p': port = atoi(optarg); continue;
			case 'h': irc_help(); 
                        case '?': irc_help();
                } 

	irc_client(name, server, port);
	return 0;
	
} 

void irc_client(char *name, char *server, int port)
{
	
	size_t y = 0, len = 8190; 
        char *in = malloc(len + 1); 
	char inpt[len], out[len + 1], nick[len], user[len], join[len], temp[6]; 
        char *token; 
        struct sockaddr_in servaddr;
        int sck; 

	if ( !server || !name || !sel.channel || !port )
                irc_help();

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        sck = socket(AF_INET, SOCK_STREAM, 0);
        inet_pton(AF_INET, server, &servaddr.sin_addr);
        connect(sck, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

        sprintf(nick, "NICK %s\r\n", name);
        sprintf(user, "USER %s 8 * : \r\n", name);
        sprintf(join, "JOIN %s\r\n", sel.channel);

        send(sck, nick, strlen(nick), 0);
        send(sck, user, strlen(user), 0);
        send(sck, join, strlen(join), 0);


        while ( 1 )
        {
                pid_t  pid;
                pid = fork();

                if (pid == 0)
                { 
                        memset(in, 0, len);
                        y = read(sck, in, len); 
                       	process_in(in, y); 
                        if (strstr(in, "PING") != NULL) 
                                send(sck, "PONG %s\r\n", 11, 0); 
                        if ( sel.off[1] == 1 )
			{
				free(in);
				close(sck); 
				kill(pid , SIGTERM);
			}
			printf("\n"); 
                }
                else
                {
			printf(colors.gre);
			printf("[%s] ", sel.channel); 
			printf(colors.mag);
                        memset(inpt, 0, len);
                        fgets(inpt, len, stdin);
			printf(colors.res);
                        if ( strlen ( inpt ) < 2 )
                                continue;

                        inpt[strlen(inpt) - 1 ] = '\0';
                        sprintf(temp, "%.5s", inpt); 
		
                        if ( strcmp(temp, "/exit") == 0 )
                        {
                                sel.off[1] = 1;
                                send(sck, "QUIT \r\n", strlen("QUIT \r\n"), 0);
                                printf("goodbye\n");
                                continue;
                        }

                        if ( strcmp(temp, "/join") == 0 )
                        {
                                token = strtok(inpt, " ");
                                while ( token != NULL )
                                {
                                        strcpy(sel.channel, token);
                                        token = strtok(NULL, " ");
                                }
                                sprintf(join, "JOIN %s\r\n", sel.channel);
                                send(sck, join, strlen(join), 0);
                                continue;
                        }
			
                        memset(out, 0, len  + 1);
                        sprintf(out, "PRIVMSG %s :%s\r\n",sel.channel, inpt); 
                        send(sck, out, strlen(out), 0);
                }
        }
        close(sck);
} 

void irc_help(){
        printf ("Usage : irc [OPTIONS]\n");
	printf ("./irc -s 38.229.70.22 -p 6667 -n nick -c '#frogs'\n "); 
	printf ("\t-h\tThis menu\n");
	printf ("\t-s\tServer\n");
	printf ("\t-p\tPort\n");
	printf ("\t-n\tName\n");
	printf ("\t-c\tChannel\n"); 
	printf ("\t/join\t Join a channel\n");
	printf ("\t/exit\t Kill the program\n");
        exit (0);
} 

void process_in(char *in, size_t y)
{ 
	size_t x, p, z, len = 8190; 
        x = p = z = 0; 
	char *tempor = malloc(len);

	for ( ; z < y ; )
	{
		if (z == 0) 
			printf("\n");
		if ( z == 0 || in[z] == '\n' )
		{ 
			memset(tempor, 0, len);
			p = x = 0; 
			for (z += 1; in[z  + 1] != ':' && z < y ; ) 
			{ 
				if (in[z] == ' ' ) ++p; 
				tempor[x] = in[z];
				tempor[x + 1] = '\0'; 
				if ( p == 3 ) break;
				++x;
				++z;
			} 
			print_time(); 
			detect_message(tempor); 	
			if ( in[z + 1] == ':' )
				in[z + 1] = ' ';
			
		} 
                printf("%c", in[++z]);
	}
	free(tempor);
}

void detect_message(char *string)
{
	size_t len = 8190;
	int a, c, d;
	a = c = d = 0; 
	char user[len];
	char chan[len];
	memset(user, 0, len); 
        memset(chan, 0, len);

	if (strstr(string, "001") != NULL) ;
	else if (strstr(string, "002") != NULL) ;
	else if (strstr(string, "003") != NULL) ;
	else if (strstr(string, "004") != NULL) ;
	else if (strstr(string, "005") != NULL) ;
	else if (strstr(string, "005") != NULL) ; // Server Message 
	else if (strstr(string, "353") != NULL) ; // Name list
	else if (strstr(string, "366") != NULL) ; // End of name list
	else if (strstr(string, "NOTICE") != NULL) ; // Notice
	else if (strstr(string, "PRIVMSG") != NULL) {
		for ( ; string[c] != '!' ; ) 
			user[d++] =  string[c++] ; 
		for ( a = 0; a < 2 ; ++c) 
			if ( string[c] == ' ' )
				++a; 
		for ( d = 0; string[c] != '\0' ; ) 
                        chan[d++] =  string[c++] ; 
		printf(" <");
		printf(colors.cya);
		printf("%s@", user);
		printf(colors.res);
		printf("%s", chan); 
		printf("> "); 
		printf(" ");
	} 

	else if (strstr(string, "PING") != NULL) ; 
	else if (strstr(string, "250") != NULL) ;
	else if (strstr(string, "251") != NULL) ;
	else if (strstr(string, "252") != NULL) ;
	else if (strstr(string, "253") != NULL) ;
	else if (strstr(string, "254") != NULL) ;
	else if (strstr(string, "255") != NULL) ;
	else if (strstr(string, "265") != NULL) ; // printf("STATS: ");
	else if (strstr(string, "266") != NULL) ; // port number ?
	else if (strstr(string, "332") != NULL) ;
	else if (strstr(string, "372") != NULL) ; // interupts server message
	else if (strstr(string, "375") != NULL) ; // MOTD
	else if (strstr(string, "376") != NULL) ; // End of MOTD
	else { 
		printf(colors.yel);
		printf("%s", string);
		printf(colors.res);
	} 
}

void print_time()
{ 
        struct tm *t;
	time_t now = time(NULL);
        t = localtime( &now ); 
	printf(colors.blu);
	printf("%.2d:%.2d:%.2d ", t->tm_hour, t->tm_min, t->tm_sec );
	printf(colors.res);

}
