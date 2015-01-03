#include <stdio.h>
#include <dirent.h> 
#include <string.h> 
#include <stdlib.h>
#include <unistd.h> 

/* (Copyright) 2014, MIT license, "posixrm.c", csitd */ 



void remove_pattern(char *);


int main(int argc, char *argv[])
{ 

	int o, recur = 0;
	
        while ((o = getopt (argc, argv, "rRh")) != -1)
                switch (o) { 
			case 'r': recur = 1; continue;
			case 'R': recur = 1; continue;
			case 'h': printf(" rm -rR [TARGET(S)]\n");
				  return 0;
                        default: break;
                }
	
	argv += optind;
        argc -= optind; 
	
	while (*argv && recur == 1) 
	{
		while ( access(*argv, F_OK ) != -1 ) 
		{ 
			remove_pattern(*argv);
			remove(*argv); 
		}
		argv++;
	} 

	if ( recur == 0 )
		while ( *argv )
                        remove(*argv++);

	return 0;
} 

void remove_pattern(char *path)
{ 
	DIR *dir;
	struct dirent *dentry;
	char spath[10000];
	char cache[10000]; 

	size_t len = strlen(path);
	if ( path[len - 1] == '/' && len > 1 )
			path[len - 1] = '\0';
	
	if ( ( dir = opendir(path) ) )
	{
		dentry = readdir(dir);
		while ( dentry )
		{
			if ( len > 1 )
				sprintf(spath, "%s/%s", path, dentry->d_name);
			else 
				sprintf(spath, "%s%s", path, dentry->d_name); 
			 
			if ( ( strcmp( ".", dentry->d_name) != 0) &&
                           ( strcmp( "..", dentry->d_name) != 0))
                        {
				if ( dentry->d_type == DT_DIR )
        	                        strcpy(cache, spath); 
				remove(spath); 
			}
			
			remove(cache); 
			
			if ( dentry->d_type == DT_DIR &&
			   ( strcmp( ".", dentry->d_name)) &&
			   ( strcmp( "..", dentry->d_name))) 
			{ 
				remove_pattern(spath); 
			} 
			dentry = readdir(dir);
		}
	}
	closedir(dir);
}


	

