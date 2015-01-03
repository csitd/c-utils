#include <stdio.h>
#include <string.h> 
#include <netdb.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 

#include <arpa/inet.h>

/* (Copyright) 2014, MIT license, "ircbot.c", csitd */

void ircbot_help();
void ircbot_error(char *); 

int main(int argc, char **argv) 
{

	size_t len = 4096;
	char in[len + 1], out[len + 1]; 
	char nick[len];
	char user[len];
	char join[len]; 
	char *server, *channel, *name; 
	int o, sck, port; 
	struct sockaddr_in servaddr; 
	char *inpt;

        while ((o = getopt (argc, argv, "hs:n:c:p:")) != -1)
                switch (o) {
                        case 's': server = optarg; continue;
                        case 'n': name = optarg; continue;
                        case 'c': channel = optarg; continue;
                        case 'p': port = atoi(optarg); continue;
			case 'h': ircbot_help(); 
                        case '?': ircbot_error("Argument is incorrect\n"); 
                } 

	if ( !server || !name || !channel || !port ) 
		ircbot_error("An Argument is incorrect\n"); 

	memset(&servaddr, 0, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port); 
	sck = socket(AF_INET, SOCK_STREAM, 0); 
	inet_pton(AF_INET, server, &servaddr.sin_addr); 
	connect(sck, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

	/* set the nick/user names and join a channel */
	sprintf(nick, "NICK %s\r\n", name); 
	sprintf(user, "USER %s 8 * : \r\n", name); 
	sprintf(join, "JOIN %s\r\n", channel); 

	send(sck, nick, strlen(nick), 0);
	send(sck, user, strlen(user), 0);
	send(sck, join, strlen(join), 0); 
	memset(in, 0, len + 1);
	while ( 1 ) 
	{ 
		pid_t  pid; 
   		pid = fork(); 

		if (pid == 0)
		{ 
			/* Child process */ 
			memset(in, 0, len + 1);
			memset(out, 0, len + 1); 
 			read(sck, in, len); 
			//printf("\nJUNCTURE ONE\n");
			printf("\n%s", in); 
			//printf("\n%s\n", out); 
			
			if (strstr(in, "PING") != NULL) 
				send(sck, "PONG %s\r\n", 11, 0); 
			if (strstr(in, "hey") != NULL) 
			{ 
                        	sprintf(out, "PRIVMSG %s :hey\r\n", channel); 
                        	send(sck, out, strlen(out), 0); 
                	} 
			if (strstr(in, "weather") != NULL) 
			{ 
                       		sprintf(out, "PRIVMSG %s :look outside\r\n", channel); 
                        	send(sck, out, strlen(out), 0); 
                	} 
		}
		else
		{
        		printf("%s$ ", name); 
			inpt = malloc(len);
			memset(inpt, 0, len); 
			fgets(inpt, len, stdin); 
			if ( strlen ( inpt ) < 2 )
				continue;
			printf("%s", inpt);
                	sprintf(out, "PRIVMSG %s :%s\r\n", channel, inpt); 
                	send(sck, out, strlen(out), 0); 
		}
	}
	close(sck);
	return 0;
} 

void ircbot_error(char *errmess){
        fprintf (stderr, "%s", errmess); 
        exit (1);
}

void ircbot_help(){
        printf ("Usage : ircbot [OPTIONS]\n");
	printf ("./ircbot -s 38.229.70.22  -p 6667 -n somenick -c '#botwars' ");
	printf ("\t-h\tThis menu\n");
	printf ("\t-s\tServer\n");
	printf ("\t-p\tPort\n");
	printf ("\t-n\tName\n");
	printf ("\t-c\tChannel\n"); 
        exit (0);
} 

