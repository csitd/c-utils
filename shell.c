#include <stdio.h>
#include <errno.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h> 

/* Needed for freebsd 10 */ 
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>


/* 
	(Copyright) 2014, MIT license, "shell.c", csitd 
  
	Usage: ./shell "[COMMANDS]"  non-interactive mode
        	 ./shell               interactive mode

	Compile with: cc shell.c -Wall -Werror -o shell

	Builtin words/features/commands:

		||
		&&
		;
		|
		&
		<
		>
		>>
		pwd
		cd
		exit 
        
*/ 

struct trans
{ 
	int pass; 
	int readin[1024];
	int writeout[1024];
	pid_t pids[1024];
	char output[1024];
	char newput[1024][1024]; 
} trans;

struct status
{
	int err;
} status;

struct states
{
	int comnum;
	int appender;
	int runmode;
	int badmode;
	int piped;
	int badred;
	char *outfp;
	char *infp; 
	int truth[1024]; 
}states; 

typedef struct command_t
{
	char *file; 
	char *args[1024];
	int argnum; 
	int inrd; 
	int outrd; 
}command_t; 


void setsignals();

void draw_prompt(void);
int bad_symbol(char *); 
size_t monads(char *);
void expand_tokens(char *, char*);
void execute(command_t [] ,char *, char *, int); 
void process_input(command_t [], char *, char [], int); 


int main(int argc, char *argv[])
{

	unsigned int i = 0;
	unsigned int j = 0;
	size_t len = 1024;
    	command_t comtyp[40];
    	char *cmnd = NULL; 
	char inpt[len]; 
	char buf[len];
	trans.pass = 0; 
	setsignals(); 

    	while (1)
    	{
		states.comnum = 0;
		states.piped = 0;
		states.runmode = 0;
        	states.infp = NULL;
		states.outfp = NULL;
		states.badred = 0;
		states.badmode = 0;

		for ( i = 0; i < 40 ; i++)
		{
            		comtyp[i].file = NULL;
            		for(j=0; j < len; j++)
               			comtyp[i].args[j] = NULL;
            		comtyp[i].argnum=0;
            		comtyp[i].inrd=2;
            		comtyp[i].outrd=2;
		}
        
	
		/* -c functionality */ 
		if ( argc > 1) 
			process_input(comtyp, cmnd, argv[1], argc); 

		/* interactive prompt */ 
		if ( argc == 1 )
			process_input(comtyp, cmnd, inpt, 0); 

		
		if(comtyp[0].file == NULL) 
    			continue;
	
		/* Builtins */
		else if(strcmp(comtyp[0].file, "exit") == 0 ) 
    			break; 
		else if(strcmp(comtyp[0].file, "cd") == 0 ) 
			chdir(comtyp[0].args[1]); 
		else if(strcmp(comtyp[0].file, "pwd") == 0 ) 
		{ 
			getcwd(buf, len);
                        printf("%s\n", buf); 
		}

		else 	if(states.badmode == 0 && states.badred == 0) 
			execute(comtyp, states. infp, states.outfp, states.runmode);
	
	} 
	return 0;
} 

void process_input(command_t cmds[], char* cmnd, char inpt[], int argcc)
{

	states.appender = 0;
	int argcount=0; 
	int cmdcount=0; 
	static size_t b =0; 
	size_t len = 1024; 
	static size_t e = 1; 
	size_t tr = 0;
	size_t a = 0; 

	if ( argcc == 0 && b == 0 )
	{ 
		draw_prompt();
		fgets(inpt, len, stdin); 
		for ( ; a < len ; a++) 
			if (inpt[a] == '\n' ) 
				while ( a < len )
					inpt[a++] = '\0'; 
		status.err =0; 
		trans.pass = 0;
		while ( tr <= len )
			states.truth[tr++] = 0; 
	} 

	if ( trans.pass == 0 )
	{
		expand_tokens(inpt, trans.output); 
		b = monads(trans.output); 
		trans.pass = 1;
	} 

	if ( e < b  +  1) 
	{ 
		if  ( (states.truth[e-1] == 2 && status.err == 0) ||
		 (states.truth[e-1] == 0) || (states.truth[e-1] == 1 && 
		 status.err > 0 )) 
		{ 
			cmnd = strtok(trans.newput[e++]," "); 
		}
		else { 
			++e; 
			return; 
		}
	}
	else 
	{ 
		e = 1; 
		b = 0; 
		if ( argcc > 0 )
			exit (0); 
		return; 
	}  

	++states.comnum; 

	cmds[cmdcount].file = cmnd;
	cmds[cmdcount].args[argcount] = cmnd;
	cmds[cmdcount].argnum++; 
	cmnd = strtok(NULL, " "); 
	while(cmnd != NULL)
	{   
		if(strcmp(cmnd,"|") == 0) 
		{ 
			states.piped = 1;
			cmnd = strtok(NULL, " "); 
			if(cmnd != NULL)
			{ 
				if ( ! (bad_symbol(cmnd )) )
    				{ 
					states.piped = 0; 
					states.badmode = 1;
					cmnd = NULL; 
				}
				else
				{ 
					cmdcount++; 
					states.comnum++;
					cmds[cmdcount].file = cmnd;
					cmds[cmdcount].args[0] = cmnd;
					cmds[cmdcount].argnum++;
				}
			}
			else
			{ 
				states.piped = 0; /* fail pipes with no arg */ 
				states.badmode = 1;
				cmnd = NULL; 
			}
		}
		else if(strcmp(cmnd,">") == 0 || strcmp(cmnd,">>") == 0 ) 
		{
			if(strcmp(cmnd,">>") == 0 ) 
				states.appender = 1;
		
			cmnd = strtok(NULL, " "); 
			if(cmnd != NULL)
			{   
				if ( !(bad_symbol(cmnd )) )
    				{ 
					states.badred = 1;
					cmnd = NULL; 
				}
				/* if redirection hasn't been filled */
				else if(states.outfp == NULL || states.infp == NULL) 
					states.outfp = cmnd; /* first redirection */
				else /* both redirections are filled */
				{ 
					states.badred = 1;
					cmnd = NULL; 
				} 
			}
			else 	states.badred = 1;
		} 
		else if (strcmp(cmnd, "<") == 0) 
		{ 
			cmnd = strtok (NULL, " "); 
			if (cmnd != NULL)
			{ 
				if ( ! (bad_symbol(cmnd )) )
    				{ 
					states.badred = 1;
					cmnd = NULL; 
				}
				else if(states.outfp == NULL || states.infp == NULL) 
					states.infp = cmnd; 
				else
				{ 
					states.badred = 1;
					cmnd = NULL; 
				}
		
			}
			else 	states.badred = 1;
		} 
		else if ( strcmp(cmnd, "&") == 0 ) 
		{
			states.runmode = 1; /* background mode */ 
			cmnd = strtok(NULL, " ");
			if( cmnd != NULL)
			{ 
				if ( ! (bad_symbol(cmnd )))
    				{ 
					states.badmode = 1;
					cmnd = NULL; 
				}
			}
		} 
		else 
		{ 
    			argcount++; /* associate args with command */
    			cmds[cmdcount].args[argcount] = cmnd;
    			cmds[cmdcount].argnum++; 
		}
		cmnd = strtok(NULL, " "); /* next token */ 
	} 
} 



void execute(command_t cmds[], char* infile, char* outfile, int bg)
{
	int pidcnt = 0; 
	struct rusage timer; 
	int fid1 = -1; 
	int fid2 = -1; 
	int fildes[2];
	pid_t pid = 0;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH; 
	int j, k, h, p, m;
	j = k = h = p = m = 0;
	status.err = 0;

	for (; p < states.comnum ; p++ )
	{
		trans.readin[p] = -1; /* no pipe or redirection of stdin */
		trans.writeout[p] = -1; /* no pipe or redirection of stdout */
	}

	if ( infile != NULL ) /* < */
	{ 
		fid1 = open(infile, O_RDONLY);
    		if(fid1 == -1)
    		{
			fprintf(stderr,"'%s' File not found.\n", infile); 
			return;
    		} 
    		trans.readin[0] = fid1;
	}
	if ( outfile != NULL ) /* > and >> */
	{ 
		if ( states.appender == 1 )
		{
			fid2 = open(outfile,O_RDWR|O_CREAT|O_APPEND,mode);
                	if(fid2 == -1)
                	{
				fprintf(stderr," Permission denied?\n"); 
                       	 	return;
                	} 
			trans.writeout[states.comnum - 1] = fid2;
		} 
		else
		{ 
			fid2 = open(outfile,O_TRUNC|O_RDWR|O_CREAT,mode);
    			if(fid2 == -1)
    			{ 
				fprintf(stderr," Permission denied?\n");
				return;
    			} 
			trans.writeout[states.comnum- 1] = fid2;
		}
	}	
	if(states.piped == 1 )
	{ 
		for(j=0 ; j < states.comnum - 1; j++)
    		{   
        		if ( pipe(fildes) == -1 ) 
            			return;
        
        		trans.readin[j+1] = fildes[0];
        		trans.writeout[j] = fildes[1];
    		}
	} 
	for(k = 0; k < states.comnum; k++)
	{
    		pid = fork();
    		if ( pid < 0 ) 
        		return; 

		else if ( pid > 0 ) /* parent */
    			trans.pids[k] = pid; 

		else /* child */
		{ 
			if(trans.writeout[k] != -1) /* redirect stdout */
				dup2(trans.writeout[k],STDOUT_FILENO);
		
			if(trans.readin[k] != -1) /* write out at first child */
				dup2(trans.readin[k],STDIN_FILENO); 
			
			for(h= 0; h < states.comnum && trans.writeout[h] != -1 ; h++) 
			{
    				close(trans.writeout[h]);
				if ( trans.readin[h] != -1 )
    					close(trans.readin[h]);
			}
			execvp(cmds[k].file, cmds[k].args); 
		
			fprintf (stderr, "shell: %d: %s: not found\n",k+1, cmds[k].file);
			
			exit(1);
		}
	} 
	
	for(m=0; m < states.comnum ; m++)
	{ 
		if(bg == 0) /* standard process */
		{
			wait4(trans.pids[m],&status.err,0,&timer); 
			/* if done close stdin and sdtout */
			if( trans.writeout[m] != -1 ) 
				close(trans.writeout[m]);
			if( trans.readin[m] != -1 ) 
				close(trans.readin[m]); 
		} 
		else 	
		{
			fprintf (stderr, "[%d]\n", trans.pids[m]);
			pidcnt++; /* background process */ 
		}
	}	
} 

int bad_symbol(char *cmnd)
{
	if( strcmp(cmnd,"|") == 0 || strcmp(cmnd,"&") == 0 ||
                strcmp(cmnd,"<") == 0 || strcmp(cmnd,">") == 0)
			return 0;
	return 1; 
}

size_t monads(char *src)
{
	size_t len = 1024; 
        size_t a = 1; 
	char *token;
	char args[len];
	memset(args, 0, len); 
        strcpy(args, src); 
	token = strtok(args, " "); 
	memset(trans.newput[a], 0, len);

        while ( token != NULL )
        { 
                if (strcmp(token, ";") == 0 ) 
		{
			memset( trans.newput[a + 1], 0, len);
			states.truth[++a - 1] = 0; 
		}
                else if (strcmp(token, "||") == 0) 
		{
			memset( trans.newput[a + 1], 0, len);
			states.truth[++a - 1] = 1; 
		}
                else if (strcmp(token, "&&") == 0) 
		{
			memset( trans.newput[a + 1], 0, len);
			states.truth[++a - 1] = 2; 
		}
                else {
			sprintf(trans.newput[a], "%s %s", trans.newput[a], token ); 
		}

                token = strtok(NULL, " "); 
        } 
        return a;
}


void expand_tokens(char *s, char* output)
{


        size_t len = 1024;
        size_t i = 0;
        size_t j = 0;
        while ( i < len - 1 )
        {
		if ( i == len ) 
		{
			output[j] = '\0' ;
			return;
		}
		if ( s[i] == '\0' )
		{ 
			output[j] = s[i];
			return;
		} 
                if ( s[i] == ';' )/* ; */ 
                {
                        output[j] = ' ';
                        output[++j] = ';';
                        output[++j] = ' ';
                } 
                else if (s[i] == '<' )/* < */
                {
                        output[j] = ' ';
                        output[++j] = '<';
                        output[++j] = ' ';
                } 
                else if (i != 0 && s[i-1] != '|' && s[i] == '|'
                         && s[i+1] != '|')  /* | */
                {
                        output[j] = ' ';
                        output[++j] = '|';
                        output[++j] = ' ';
                } 
                else if ( s[i] == '|' && s[i+1] == '|' )  /* || */
                {
                        output[j] = ' ';
                        output[++j] = '|';
                        output[++j] = '|';
                        output[++j] = ' ';
                        ++i;
                } 
                else if ( s[i] == '&' && s[i+1] == '&' )  /* && */
                {
                        output[j] = ' ';
                        output[++j] = '&';
                        output[++j] = '&';
                        output[++j] = ' ';
                        ++i;
                } 
                else if (i != 0 &&  s[i-1] != '&' && s[i] == '&'
                         && s[i+1] != '&')  /* & */
                {
                        output[j] = ' ';
                        output[++j] = '&';
                        output[++j] = ' ';
                } 
                else if (s[i] == '>' && s[i+1] == '>' )/* >> */
                {
                        output[j] = ' ';
                        output[++j] = '>';
                        output[++j] = '>';
                        output[++j] = ' ';
                        ++i;
                } 
                else if ( i != 0 && i != 0 && s[i-1] != '>' && s[i] == '>'
                         && s[i+1] != '>' )	/* > */
                {
                        output[j] = ' ';
                        output[++j] = '>';
                        output[++j] = ' ';
                } 
                else 
		{
			output[j] = s[i];/* normal */
			output[j+1] = '\0';
		}
                ++j;
                ++i; 
        }
}


void setsignals()
{
	signal (SIGINT, SIG_IGN);
}

void draw_prompt(void)
{
	size_t len = 1024;
	char cwd[len];
        char hostname[len];
	memset(cwd, 0, len);
	memset(hostname, 0, len);
	getcwd(cwd, len); 
        gethostname (hostname, len); 
        printf ("%s", getenv("USER"));
        printf ("@");
        printf ("%s:", hostname);
        printf ("%s$ ", cwd); 
}
