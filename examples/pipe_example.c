#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
//#include <sys/wait.h> 

/* (C) 2014, MIT license, "pipe_example.c", C. Graff */ 

size_t process_args(char *[], char *); 
int handle_pipes(char **, int , char **);

int main(void)
{ 
	int filedes[2]; 
	size_t length = 1000;
        char *buffer = malloc(length);
        char *args[length];
	char *dargs[length];
	int status = 0; 

        while (1)
        { 
		pipe(filedes); 

                printf("$ "); 
                fgets(buffer, length, stdin); 
		size_t argnum = process_args(args, buffer); 
		if (argnum == 0)
                        continue; 
	
		if ( handle_pipes(args, argnum, dargs) )
		{ 
    			pid_t pid = fork(); 
    			if (pid == 0) { 
        			dup2(filedes[1], 1); 
				execvp(args[0], args); 
    			} else { 
        			close(filedes[1]);
    			}
			wait(pid);
    		} else  {
			int p = 0, o = 0; 
			while ( p < 1000 )
                                dargs[p++] = args[o++]; 
		} 
		
                pid_t pid2 = fork(); 
    		if (pid2 == 0) { 
			dup2(filedes[0], 0); 
			execvp(dargs[0], dargs);
			close(filedes[1]); 
    		} 
		wait(pid2); 
	}
}

size_t process_args(char *args[], char *buffer)
{
        char *buf_args[1000];
        char **cp;
        char *wbuf; 
        memset(&wbuf, 0, 1000);
        int k = 0;
        while ( k < 1000 )
        {
                buf_args[k] = '\0';
                args[k] = NULL ;
                ++k;
        }
        size_t argnum = 0, i = 0;
        wbuf = buf_args[0] = args[0] = buffer;

        cp = buf_args;

        while((*cp = strsep(&wbuf, " \n\t")) != '\0')
                if (++cp >= &buf_args[1000])
                        break;

        while (strlen(buf_args[i]) > 0 )
                args[argnum++] = buf_args[i++];
        return argnum;
}

int handle_pipes(char **args, int argnum, char **dargs)
{
        int l = 0;
        int m = 0;
        int p = 1;
        size_t len = 1000;
        while ( l < argnum )
        {
                if ((strncmp(args[l], "|" , len) == 0))
                {
                        while ( p < argnum )
                                dargs[m++] = args[l + p++];
                        args[l] = NULL; 
                        return l;
                }
                ++l;
        }
        return 0;
}
