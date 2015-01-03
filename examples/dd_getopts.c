#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

/* (C) 2014, MIT license, "dd_getopts.c", C. Graff */
void local_error();
int isnota_num(char d); 
void multiplier_table(char *g, char *e, char *l);

void conv_table(char *o, char *p);
void convl_table(char *o, char *p);
void iflags_table(char *o, char *p);
void oflags_table(char *o, char *p);
void input_file(char *o, char *p);
char inputfile;


unsigned long count, obs, ibs, skip, seek, bs; 


int main ( int argc, char *argv[] )
{
   
     char* convs = NULL;
     char* argname = NULL;
     const char *verbatim;
     char* argstring = NULL;

     int argcounter = 1; 
     int ret = 0; 
     size_t len = 0; 
     int tokopt = 0; 
     char* numerical = NULL;

     while ( argcounter < argc ) {  
          
          /* these 5 vars have to be reset every pass */ 
          /* tokopt ? */
          int badopt = 1;
          int f = 0;
          int z = 0; 
          int a = 0;
          int olda = 0; 
         

          /* get the initial argument  */
          len = strlen(argv[argcounter] ); 
          verbatim = argv[argcounter]; 

          /* tokenize the "=" sign, and the point numerical args transition to chars */
          while ( f < len ) { 
               if ( verbatim[f] == '=' ) { tokopt = f; badopt = 0 ; } 
               ret = isnota_num(verbatim[f]);
               if ( ret == 1 && badopt == 0 && verbatim[f] != '=') { ++z; } 
               ++f; 
          } 
          
          /* if arg contained no "=" sign then exit */
          if ( badopt != 0 ) { local_error(); } 

          /* copy non-integer argument strings. ie; "sync" or "M" */ 
          argstring = malloc(z + 1);
          memcpy (argstring, &verbatim[len - z], z); 
          argstring[z] = '\0'; 
          
          /* copy the actual argument name. ie; "count" or "conv" */
          argname = malloc(tokopt + 1 );
          memcpy(argname, &verbatim[0], tokopt); 
          argname[tokopt] = '\0'; 

          /* copy any numerically based arguments. ie; "1000"  */
          numerical = malloc(len - tokopt + 1); 
          memcpy(numerical, &verbatim[tokopt  + 1], len - tokopt ); 
          numerical[len - tokopt - 1] = '\0';
          numerical[len - tokopt] = '\0'; 

          /* check that the third array does not exceed 19 digits */
          if ((strlen(numerical)) > 19 + z) { local_error(); } 

          /* apply multipliers to any numerical arguments  */
          multiplier_table(argstring, argname, numerical);


          /* work with comma based lists  */ 
          while (argstring[a] != '\0' ) {
               ++a; 
               if ( argstring[a] == ','  || argstring[a] == '\0') {
			/* this needs a sizeof */
                    convs = malloc(a - olda + 1);
                    memcpy(convs, &argstring[olda], a - olda);
                    convs[a - olda] = '\0';
                    olda = a + 1; 
                    if ( strcmp("conv", argname) == 0 ) { 
			conv_table(convs, argname);
                         printf ("  conv=  %s\n", convs); 
                    }
                    if ( strcmp("iflags", argname) == 0 ) { 
			iflags_table(convs, argname);
                         printf ("  iflags=  %s\n", convs); 
                    } 
                    if ( strcmp("oflags", argname) == 0 ) { 
			oflags_table(convs, argname);
                         printf ("  oflags=  %s\n", convs);
                    } 
               } 
          } 
          
          /* handle input file  */
          input_file(argname, argstring);

          ++argcounter; 

     }
     
     
     /* report back options which have been discovered */ 
     printf ("count=%lu\n", count); 
     printf ("skip=%lu\n", skip); 
     printf ("seek=%lu\n",seek); 
     printf ("ibs=%lu\n",ibs); 
     printf ("obs=%lu\n",obs); 
     printf ("bs=%lu\n",bs); 
             
     return 0;
}


int isnota_num(char d)
{ 
     if ( d != '0' && d != '1' && d != '2' && d != '3' && 
          d != '4' && d != '5' && d != '6' && d != '7' && 
          d != '8' && d != '9' ) { return 1;
     } else return 0;
     return 0; 
}


void multiplier_table(char *optarg, char *optname, char *numerical)
{
     int m = 1; 
     if (strcmp("c",  optarg) == 0 ) { m = 1; }
     if (strcmp("w",  optarg) == 0 ) { m = 4; }
     if (strcmp("b",  optarg) == 0 ) { m = 512; }
     if (strcmp("kB", optarg) == 0 ) { m = 1024; }
     if (strcmp("K",  optarg) == 0 ) { m = 1000; }
     if (strcmp("MB", optarg) == 0 ) { m = 1000000; }
     if (strcmp("M",  optarg) == 0 ) { m = 1000000; }
     if (strcmp("xM", optarg) == 0 ) { m = 1000000; }
     if (strcmp("GB", optarg) == 0 ) { m = 1000000000; }
     if (strcmp("G",  optarg) == 0 ) { m = 1000000000; }
     if ( strcmp("count",      optname) == 0 ) { count = ((atol(numerical)) * m); 
     } else if (strcmp("skip", optname) == 0 ) { skip = ((atol(numerical)) * m);
     } else if (strcmp("seek", optname) == 0 ) { seek = ((atol(numerical)) * m); 
     } else if (strcmp("ibs",  optname) == 0 ) { ibs = ((atol(numerical)) * m); 
     } else if (strcmp("obs",  optname) == 0 ) { obs = ((atol(numerical)) * m); 
     } else if (strcmp("bs",   optname) == 0 ) { bs = ((atol(numerical)) * m); 
     } 

}



void conv_table(char *convs, char *optname)
{ 
     if (strcmp("ascii", convs) == 0 ) {
          int convs_ascii = 1;
          printf ("ascii activated \n");
     } else if (strcmp("ebcdic",    convs) == 0 ) { int convs_ebcdic = 1;
     } else if (strcmp("ibm",       convs) == 0 ) { int convs_ibm = 1;
     } else if (strcmp("block",     convs) == 0 ) { int convs_block = 1;
     } else if (strcmp("unblock",   convs) == 0 ) { int convs_unblock = 1;
     } else if (strcmp("lcase",     convs) == 0 ) { int convs_lcase = 1;
     } else if (strcmp("ucase",     convs) == 0 ) { int convs_ucase = 1;
     } else if (strcmp("swab",      convs) == 0 ) { int convs_swab = 1;
     } else if (strcmp("sync",      convs) == 0 ) { int convs_sync = 1;
     } else if (strcmp("excl",      convs) == 0 ) { int convs_excl = 1;
     } else if (strcmp("nocreat",   convs) == 0 ) { int convs_nocreat = 1;
     } else if (strcmp("notrunc",   convs) == 0 ) { int convs_notrunc = 1;
     } else if (strcmp("noerror",   convs) == 0 ) { int convs_noerror = 1;
     } else if (strcmp("fdatasync", convs) == 0 ) { int convs_fdatasync = 1;
     } else if (strcmp("fsync",     convs) == 0 ) { int convs_fsync = 1;
     } 
}

void iflags_table(char *convs, char *optname) 
{ 
     if (strcmp("append", convs) == 0 ) {
          int iflags_append  = 1;
          printf (" append activated \n");
     } else if (strcmp("append",    convs) == 0 ) { int iflags_append  = 1;
     } else if (strcmp("direct",    convs) == 0 ) { int iflags_direct = 1;
     } else if (strcmp("directory", convs) == 0 ) { int iflags_directory = 1;
     } else if (strcmp("dsync",     convs) == 0 ) { int iflags_dsync= 1;
     } else if (strcmp("sync",      convs) == 0 ) { int iflags_sync = 1;
     } else if (strcmp("fullblock", convs) == 0 ) { int iflags_fullblock = 1;
     } else if (strcmp("nonblock",  convs) == 0 ) { int iflags_nonblock= 1;
     } else if (strcmp("noatime",   convs) == 0 ) { int iflags_noatime = 1;
     } else if (strcmp("nocache",   convs) == 0 ) { int iflags_nocache = 1;
     } else if (strcmp("noctty",    convs) == 0 ) { int iflags_noctty = 1;
     } else if (strcmp("nofollow",  convs) == 0 ) { int iflags_nofollow = 1; 
     } 
}

void oflags_table(char *convs, char *optname)
{ 
     if (strcmp("append", convs) == 0 ) {
          int iflags_append  = 1;
          printf (" append activated \n");
     } else if (strcmp("append",    convs) == 0 ) { int oflags_append  = 1;
     } else if (strcmp("direct",    convs) == 0 ) { int oflags_direct = 1;
     } else if (strcmp("directory", convs) == 0 ) { int oflags_directory = 1;
     } else if (strcmp("dsync",     convs) == 0 ) { int oflags_dsync= 1;
     } else if (strcmp("sync",      convs) == 0 ) { int oflags_sync = 1;
     } else if (strcmp("fullblock", convs) == 0 ) { int oflags_fullblock = 1;
     } else if (strcmp("nonblock",  convs) == 0 ) { int oflags_nonblock= 1;
     } else if (strcmp("noatime",   convs) == 0 ) { int oflags_noatime = 1;
     } else if (strcmp("nocache",   convs) == 0 ) { int oflags_nocache = 1;
     } else if (strcmp("noctty",    convs) == 0 ) { int oflags_noctty = 1;
     } else if (strcmp("nofollow",  convs) == 0 ) { int oflags_nofollow = 1; 
     } 

}

void input_file(char *argname, char *argstring)
{ 
     char buf;
     char *ptr;
     long aq, bq, cq, dq;
     int fileend,filesrt,filecur,i,j,n;


     if (strcmp("of", argname) == 0 ) {
             printf ( "of= %s \n", argstring);
     }

     inputfile = open(argstring, O_RDONLY);
      
     fileend = lseek(inputfile, (off_t) 0, SEEK_END);
     filesrt = lseek(inputfile, (off_t) 0, SEEK_SET);
     filecur = lseek(inputfile, (off_t) 0, SEEK_CUR);


     j = filesrt;
     i = 1;
     while ( j < fileend && filecur <  seek  )
     {
          ++j;
          n = read(inputfile, &buf, i);
          //if ( j > seek )
          n = write(STDOUT_FILENO, &buf, i);

          filecur = lseek(inputfile, (off_t) 0, SEEK_CUR);
     }
                   //         O_DIRECTORY
     close(inputfile);
      /* if there is no of= this should probably fall back to stdin */
}
void local_error(){ 
     fprintf (stderr, "Number too large, or invalid option. \n"); 
     exit (1);  
}




