#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "mwc.h"


static void __parse_file(int fd, mwc_file* file)
{
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];

    size_t bytes_read = 0;
    while ((bytes_read = read(fd, buf, BUF_SIZE)) != 0) {
	if (bytes_read == -1) {
	    perror("read()");
	    exit(EXIT_FAILURE);
	}

	file->bytes += bytes_read;

	for (size_t i = 0; i < bytes_read; i++) {
	    if (isspace(buf[i]) || buf[i] == '\n') {
		if (file->last_ascii) {
		    file->words++;
		    file->last_ascii = false;
		}
	    } else file->last_ascii = true;

	    if (buf[i] == '\n') file->lines++;
	}    
    }

    if (file->last_ascii) file->words++;
}


static void __wc(mwc_file* file) 
{
    __parse_file(fileno(stdin), file);
}


void parse_args(int argc, char *argv[], mwc_config *config) 
{
    config->file_count = argc - 1;

    if (config->file_count != 0)
	config->files_to_parse = argv + 1;
}


void mwc(mwc_config *config) 
{
    /* NOTE
     * If no files provided, use stdin.
     */
    if (config->file_count == 0) {
	mwc_file file = MWC_FILE_INIT;
	__wc(&file);

	printf(WC_FMT, file.lines, file.words, file.bytes, "");
	return;
    }

    for (uint64_t i = 0; i < config->file_count; i++) {
	int fd = open(config->files_to_parse[i], O_RDONLY);
	if (fd == -1) {
	    perror("open()");
	    exit(EXIT_FAILURE);
	}

	struct stat fstats;

	if (fstat(fd, &fstats) == -1) {
	    perror("fstat()");
	    exit(EXIT_FAILURE);
	}
	

	if (!S_ISREG(fstats.st_mode)) {
	    close(fd);
	    fprintf(stderr, ERROR "%s is not a regular file\n", config->files_to_parse[i]);
	    continue;
	}

	char *file_contents = (char*) mmap(0, fstats.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (file_contents == MAP_FAILED) {
	    perror("mmap()");
	    exit(EXIT_FAILURE);
	}

	mwc_file file = MWC_FILE_INIT;
	__parse_file(fd, &file);
	printf(WC_FMT, file.lines, file.words, file.bytes, config->files_to_parse[i]);

	config->total_bytes += file.bytes;
	config->total_lines += file.lines;
	config->total_words += file.words;

	if (munmap(file_contents, fstats.st_size) == -1) {
	    perror("munmap()");
	    exit(EXIT_FAILURE);
	}
    }

    if (config->file_count > 1) {
	printf(WC_FMT, config->total_lines, config->total_words, config->total_bytes, "total");
    }
}
