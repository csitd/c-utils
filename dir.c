#include <stdio.h> 
#include <dirent.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h> 
#include <pwd.h>
#include <grp.h>

/* 
   (Copyright) 2014, MIT license, "ls.c", csitd 
*/ 

void dir_help();
void print_plain(char *[], int);
void sort_strings(char *[], int); 
void octtoperm(int);
void print_strings(char *[], int, int, int);
void shift_alpha(char *[], char *[], int, int);

struct option{
	int plain;
	int alpha;
	int inode;
	int numer;
	int horiz;
	int hiddn;
	char path[1024];
} option;

struct output{ 
	char *strings[1024]; 
} output;

void list_dirs(char *);

int main(int argc, char *argv[])
{ 
	option.plain = 0;
	option.alpha = 1;
	option.inode = 0;
	option.numer = 0;
	option.horiz = 0;
	option.hiddn = 0;
	int o;

	while ((o = getopt (argc, argv, "lUinxhCa")) != -1)
                switch (o) { 
			case 'l' : option.plain = 1; break;
			case 'U' : option.alpha = 0; break;
			case 'i' : option.inode = 1; break;
			case 'n' : option.numer = 1; break;
			case 'x' : option.horiz = 1; break;
			case 'C' : option.plain = 0; break;
			case 'a' : option.hiddn = 1; break;
			case 'h' : dir_help(); break;
			case '?' : return 0; 
			default: break;
                }

        argv += optind;
        argc -= optind;


	if (argc == 0 ) 
	{
		strcpy(option.path, ".");
		list_dirs(".");
	}
	while (argc > 0 && *argv )
	{
		strcpy(option.path, *argv);
		list_dirs(*argv++); 
	}

	return 0;
} 


void list_dirs(char *argvv) 
{ 
	struct winsize w;
	DIR* a;
        struct dirent* b; 
	size_t max = 1;
        size_t hold = 1;
	size_t len = 1024;
	char *strings[len]; 
	int ct, c, factor, refactor, z, i;
	ct = c = factor = refactor = z = i = 0; 
	
	/* Discover and count directory entries */
	if ((a = opendir(argvv)))
	{
                while ((b = readdir(a)))
		{ 
			if (( option.hiddn != 1 && b->d_name[0] == '.' ))
				continue;

			strings[c] = malloc(len); 
			memset(strings[c], 0, len); 
			strcpy(strings[c], b->d_name); 
			hold = strlen(strings[c]);
                       	if ( hold > max )
                               	max = hold;
			++c; 
		} 
		closedir(a); 
	}
	
	/* Obtain terminal information */ 
	ioctl(0, TIOCGWINSZ, &w); 
	factor = w.ws_col / max ;
	refactor = ( w.ws_col - factor ) / max ; 

	/* Alphabetize discovered entries */
	if ( option.alpha == 1 )
		sort_strings(strings, c); 

	if ( option.plain == 1 ) 
		print_plain(strings, c); 

	if ( option.plain == 0 )
	{
		if ( option.horiz == 1 )
		{
			print_strings(strings, c - refactor - 1, refactor, max);
			printf("\n"); 
		}
		else {
			shift_alpha(output.strings, strings, c, refactor); 
			print_strings(output.strings, c, refactor, max); 
			for (ct = 0; ct <= c + refactor; ++ct) 
				free(output.strings[ct]); 
		} 
	} 

	for (ct = 0; ct < c ; ++ct) 
		free(strings[ct]); 

}


void shift_alpha(char *output[], char *strings[], int c, int refactor)
{
	/* Format columnar lists to alphabetize vertically */
	size_t len = 1024;
	int cnt, sft, sftc, vary = 0;
        cnt = sft = sftc = vary = 0;
        while ( cnt < c )
        {
                output[sft] = malloc(len);
                memset(output[sft] , 0, len);
                strcpy(output[sft], strings[cnt]);
                sft = (sft + refactor);
                ++sftc;
                if (sftc == ( c / refactor ) + 1)
                {
                        ++vary;
                        sft = vary;
                        sftc = 0;
                }
                ++cnt;
        } 
}

void sort_strings(char *strings[], int c)
{
	int i, j;
	char t[1024];
	for (i = 1; i < c; i++)
        {
                for (j = 1; j < c; j++)
                {
                        if (strcmp(strings[j - 1], strings[j]) > 0)
                        {
                                strcpy(t, strings[j - 1]);
                                strcpy(strings[j - 1], strings[j]);
                                strcpy(strings[j], t);
                        }
                }
        }
}

void print_strings(char *strings[], int c, int refactor, int max)
{
	int i, z = 0;

        for (i = 0; i <= c + refactor; i++)
        { 
		if ((strings[i])) 
                	printf("%-*s ", max , strings[i]); 
                ++z; 
                if ( z == refactor )
                {
                        printf("\n");
                        z =0;
                } 
        } 
}

void print_plain(char *strings[], int c)
{
	struct stat sb;
	int i = 0;
	struct group *grp;
	struct passwd *pwd;
	
	char temp[1024];
	
	while ( i < c )
	{ 
		memset(&sb, 0, sizeof(struct stat)); 
		sprintf(temp, "%s/%s", option.path, strings[i]); 
		lstat(temp, &sb); 

		/* octoperm needs an extra " " */
		octtoperm(sb.st_mode); 
		printf(" ");

		if ( option.inode == 1)
			printf("%-8ld ", (long) sb.st_ino); 
		printf("%-3ld ", (long) sb.st_nlink); 
		if ( option.numer == 1)
			printf("%ld %ld ", (long) sb.st_uid, (long) sb.st_gid);
		else {
			grp = getgrgid(sb.st_gid);
			pwd = getpwuid(sb.st_uid);
			printf("%s ", pwd->pw_name);
			printf("%s ", grp->gr_name); 
		} 
		
		printf("%8lld ", (long long) sb.st_size); 
		printf("%.16s   ", ctime(&sb.st_mtime)); 
		printf("%s\n", strings[i++]);
	}
}

void dir_help()
{
	printf("Usage:   ls -lUinxCah [PATH(S)]\n");
        printf("-l      Long listing\n");
        printf("-U      Do not alphabetize\n");
        printf("-i      Show inodes\n");
        printf("-n      Numerical group and user ID\n");
        printf("-x      Alphabetize horizontally\n");
	printf("-C      force into columns\n");
	printf("-a      list all files, even if hidden\n");
        printf("-h      Help menu\n");
	exit (0);
}

void octtoperm(int octal)
{ 
	switch (octal & S_IFMT) {	
        	case S_IFBLK:  printf("b"); break;
        	case S_IFCHR:  printf("c"); break;
        	case S_IFDIR:  printf("d"); break;
        	case S_IFIFO:  printf("p"); break; 
		case S_IFLNK:  printf("l"); break;
        	case S_IFREG:  printf("-"); break; 
        	case S_IFSOCK: printf("S"); break; 
        	default:       printf("?"); break; 

        } 
        printf( (octal & S_IRUSR) ? "r" : "-");
        printf( (octal & S_IWUSR) ? "w" : "-");
        printf( (octal & S_IXUSR) ? "x" : "-");
        printf( (octal & S_IRGRP) ? "r" : "-");
        printf( (octal & S_IWGRP) ? "w" : "-");
        printf( (octal & S_IXGRP) ? "x" : "-");
        printf( (octal & S_IROTH) ? "r" : "-");
        printf( (octal & S_IWOTH) ? "w" : "-"); 

	if ( octal & S_ISVTX && octal & S_IXOTH )
		printf("t");
	else if (octal & S_ISVTX )
		printf("T");
	else if (octal & S_IXOTH )
		printf("x");
	else	
		printf("-"); 

}

