#include <stdio.h>
#include <archive.h>
#include <archive_entry.h> 
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


/* (Copyright) 2014, MIT license, "tar.c", csitd */


struct tflags {
	int gzip; 
	int bzip2; 
	int xz; 
	int list;
	int type;
} tflags;


void tar_error(char *);
static void extract(char *);
void tarlist(char *);
void write_archive(char *, char **);
static int copy_data(struct archive *, struct archive *); 


int main(int argc, char *argv[])
{
	/* Compile with: gcc tar.c -o tar -larchive */
	//char *outname; 
	size_t len = 8192;
	char *outname = malloc(len);
	memset(outname, 0 , len);
	int o, crt = 0;
        int xtr = 0;
	tflags.gzip = 0;
	tflags.bzip2 = 0;
	tflags.xz = 0;
	tflags.list = 0;
	tflags.type = 0;

        while ((o = getopt (argc, argv, "cxfhzjJtH:")) != -1)
                switch (o) {
                        case 'c': crt = 1; continue;
                        case 'x': xtr = 1; continue;
			case 'z': tflags.gzip = 1; continue;
			case 'j': tflags.bzip2 = 1; continue;
			case 'J': tflags.xz = 1; continue;
                        case 'f': continue;
			case 't': tflags.list = 1; continue;
			case 'H': if ( strcmp("pax", optarg) == 0)
					tflags.type = 0;
				  else if ( strcmp("ustar", optarg) == 0)
					tflags.type = 1;	
				  else if ( strcmp("gnutar", optarg) == 0)
                                        tflags.type = 2;
				  else if ( strcmp("paxres", optarg) == 0)
                                        tflags.type = 3;
				  else tar_error("Unknown type\n");
				  continue;
                        case 'h': tar_error("tar -cxzjtJfhH: [OUTPUT] [FILE(S)]\n");
                        case '?': return 0;
                }

        argv += optind;
        argc -= optind; 

	outname = *argv++; 

	if ( crt == 1 && xtr == 1 ) 
                tar_error("You may not specify more than one '-xc'\n"); 

        if ( crt == 1 ) 
		write_archive(outname, argv); 

        if ( xtr == 1 ) 
		extract(outname); 
	
	if ( tflags.list == 1)
		tarlist(outname); 

	return 0;
}

void tarlist(char *string)
{
	struct archive *a;
	struct archive_entry *entry;
	int r;

	a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_all(a);
	r = archive_read_open_filename(a, string, 10240); 
	if (r != ARCHIVE_OK)
		exit(1);
	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) 
	{
		printf("%s\n",archive_entry_pathname(entry));
		archive_read_data_skip(a); 
	}
	r = archive_read_free(a); 
	if (r != ARCHIVE_OK)
		exit(1);
}

void tar_error(char *errmess)
{
        fprintf(stderr,"%s", errmess);
        exit (0);
}


void write_archive(char *outname, char **filename)
{
	struct archive *a;
	struct archive_entry *entry;
	struct stat st;
	char buff[8192]; 
	//memset(buff, 0 , 8192);
	int len;
	int fd;

	a = archive_write_new();

	/* compression types */
	if ( tflags.gzip == 1 )
		archive_write_add_filter_gzip(a);
	if ( tflags.xz == 1 )
                archive_write_add_filter_xz(a);
	if ( tflags.bzip2 == 1 )
                archive_write_add_filter_bzip2(a);

	/* tar types */
	if ( tflags.type == 0 )
		//archive_write_set_format_pax_restricted(a);
		archive_write_set_format_pax(a);
	if ( tflags.type == 1 )
		archive_write_set_format_ustar(a);
	if ( tflags.type == 2 )
                archive_write_set_format_gnutar(a);
	if ( tflags.type == 3 )
		archive_write_set_format_pax_restricted(a);

	archive_write_open_filename(a,outname);

	while (*filename) 
	{
		stat(*filename, &st);
		entry = archive_entry_new(); 
		archive_entry_set_pathname(entry, *filename);
		archive_entry_set_size(entry, st.st_size); 
		archive_entry_set_filetype(entry, AE_IFREG);
		archive_entry_set_perm(entry, 0644);
		archive_write_header(a, entry);
		fd = open(*filename, O_RDONLY);
		len = read(fd, buff, sizeof(buff));
		
		while ( len > 0 ) 
		{
			archive_write_data(a, buff, len);
			len = read(fd, buff, sizeof(buff));
		}
		close(fd);
		archive_entry_free(entry);
		filename++;
	}
	archive_write_close(a); 
	archive_write_free(a); 
}


static int copy_data(struct archive *ar, struct archive *aw)
{
	int r;
	const void *buff; 
	size_t size;
	off_t offset;

	for (;;) 
	{
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r < ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r < ARCHIVE_OK) {
			fprintf(stderr, "%s\n", archive_error_string(aw));
			return (r);
		}
	}
}
static void extract(char *filename)
{ 

	struct archive *a;
	struct archive *ext;
	struct archive_entry *entry;
	int flags;
	int r;

	/* Select which attributes we want to restore. */
	flags = ARCHIVE_EXTRACT_TIME;
	flags |= ARCHIVE_EXTRACT_PERM;
	flags |= ARCHIVE_EXTRACT_ACL;
	flags |= ARCHIVE_EXTRACT_FFLAGS;

	a = archive_read_new();
	archive_read_support_format_all(a);
	archive_read_support_compression_all(a);
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, flags);
	archive_write_disk_set_standard_lookup(ext);
	if ((r = archive_read_open_filename(a, filename, 10240)))
		exit(1);
	for (;;) 
	{
		r = archive_read_next_header(a, &entry);

		if (r == ARCHIVE_EOF)
			break;
		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(a));
		if (r < ARCHIVE_WARN)
			exit(1);

		r = archive_write_header(ext, entry);

		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(ext));
		else if (archive_entry_size(entry) > 0) 
		{
			r = copy_data(a, ext);
			if (r < ARCHIVE_OK)
				fprintf(stderr, "%s\n", archive_error_string(ext));
			if (r < ARCHIVE_WARN)
				exit(1);
		}

		r = archive_write_finish_entry(ext);

		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(ext));
		if (r < ARCHIVE_WARN)
			exit(1);
	}
	archive_read_close(a);
	archive_read_free(a);
	archive_write_close(ext);
	archive_write_free(ext);
	exit(0);
}


