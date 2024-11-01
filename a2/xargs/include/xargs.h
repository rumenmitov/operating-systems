/***
    xargs.h

    Provides initialization of the xargs config
    by parsing the options and arguments.
*/


#ifndef __XARGS_H
#define __XARGS_H

#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "queue_xargs.h"
#include "err_xargs.h"


#define DEFAULT_COMMAND "/bin/echo"


/**
 * @brief Config structure for xargs.
 * @field char command[COMMAND_MAX_LEN]
 * @field unsigned int number_of_args
 * @field bool verbose
 * @field unsigned int jobs
 * @field queue args
 */
typedef struct config 
{
    char *command;
    unsigned int number_of_command_args;
    bool command_needs_free;
    
    unsigned int number_of_args;
    queue args;

    unsigned int jobs;

    bool verbose;
} config;


/**
 * @brief Parses options and updates xargs config.
 * @param int argc
 * @param char *argv[]
 * @param config *xargs_config
 */
static void _parse_opts(int argc, char * argv[], config *xargs_config) 
{
    opterr = 0;
    
    int opt;
    unsigned int n = INT_MAX;
    unsigned int j = 1;
    

    while ((opt = getopt(argc, argv, "+n:j:t")) != -1) {
	switch (opt) {
	case '?':
	    fprintf(stderr, ERROR "Invalid argument!\n");
	    exit(EXIT_FAILURE);
	    break;

	case ':':
	    fprintf(stderr, ERROR "Missing argument!\n");
	    exit(EXIT_FAILURE);
	    break;

	case 'n':
	    n = atol(optarg);
	    if (n == 0) {
		perror(ERROR "atol()");
		exit(EXIT_FAILURE);
	    }

	    xargs_config->number_of_args = n;
	    break;

	case 't':
	    xargs_config->verbose = true;
	    break;

	case 'j':
	    j = atol(optarg);
	    if (j == 0) {
		perror(ERROR "atol()");
		exit(EXIT_FAILURE);
	    }

	    /* NOTE
	     * It doesn't make sense to have more
	     * jobs than commands that need to be
	     * executed.
	     */
	    xargs_config->jobs = (j <= n) ? j : n;
	    break;

	default:
	    break;
	}
    }


    if (optind > 0 && optind < argc) {
	xargs_config->command = argv[optind];
	xargs_config->number_of_command_args = argc - optind;
	xargs_config->command_needs_free = false;
	
    } else {
	xargs_config->command =
	    (char *) malloc( sizeof(char*) * (strlen(DEFAULT_COMMAND) + 1) );

	if (xargs_config->command == NULL) {
	    perror(ERROR "malloc()");
	    exit(EXIT_FAILURE);
	}

	strncpy(xargs_config->command, DEFAULT_COMMAND, strlen(DEFAULT_COMMAND) + 1);
	xargs_config->command[strlen(DEFAULT_COMMAND)] = '\0';
	xargs_config->command_needs_free = true;

    }
}


/**
 * @brief Helper function that returns a fresh args_buf.
 * @param config *xargs_config
 * @param char *argv[] 
 * @returns char**
 */
static char** _args_buf_init(config *xargs_config, char *argv[]) 
{
    /* NOTE
     * I allocate an additional two sizeof(char*) for the NULL at the
     * end and the command at the beginning.
     */
    unsigned long bytes_to_malloc = (xargs_config->number_of_args == INT_MAX)
	? (INT_MAX - xargs_config->number_of_command_args - 2) / sizeof(char*)
	: xargs_config->number_of_args;
    
    char **args_buf = (char**) malloc(
	sizeof(char*) *
	(bytes_to_malloc + xargs_config->number_of_command_args + 2));
    
    if (args_buf == NULL) {
	perror(ERROR "malloc()");
	exit(EXIT_FAILURE);
    }


    args_buf[0] = xargs_config->command;

    for (int i = 1; i < xargs_config->number_of_command_args; i++) {
	args_buf[i] = argv[i + optind];
    }
    
    return args_buf;
}


/**
 * @brief Initializes xargs config.
 * @param int argc
 * @param char *argv[]
 * @param config *xargs_config
 */
void xargs_init(int argc, char *argv[], config *xargs_config) 
{
    xargs_config->number_of_command_args = 0;
    xargs_config->number_of_args = INT_MAX;
    xargs_config->verbose = false;
    xargs_config->jobs = 1;
    

    _parse_opts(argc, argv, xargs_config);
	
    /* NOTE
     * Each element in xargs_config->args will contain the executable
     * program in the first position and the NULL terminator
     * in the last position.
     *
     * This is done so that it could be passed directly to execvp().
     */
    queue_init(&xargs_config->args);

    char **args_buf = _args_buf_init(xargs_config, argv);
    unsigned int args_buf_size = 0;
    unsigned int args_buf_index = (xargs_config->number_of_command_args == 0)
	? 1 + args_buf_size
	: args_buf_size + xargs_config->number_of_command_args;


    char buf[1024];
    while ((fgets(buf, 1024, stdin)) != NULL) {
	buf[strlen(buf) - 1] = '\0';

	if (strncmp(buf, "", 1) == 0) continue;
	
	args_buf[args_buf_index] =
	    (char*) malloc( sizeof(char) * (strlen(buf) + 1) );

	if (args_buf[args_buf_index] == NULL) {
	    perror(ERROR "malloc()");
	    exit(EXIT_FAILURE);
	}

	strncpy(args_buf[args_buf_index], buf, strlen(buf));
	args_buf[args_buf_index][strlen(buf)] = '\0';
	args_buf_size++;
	args_buf_index++;
	

	if (args_buf_size == xargs_config->number_of_args) {
	    args_buf[args_buf_index + 1] = NULL;

	    queue_insert(
		&xargs_config->args,
		args_buf,
		args_buf_size);

	    /* NOTE
	     * The queue is responsible for freeing
	     * args_buf!
	     */
	    args_buf = _args_buf_init(xargs_config, argv);
	    args_buf_size = 0;
	    args_buf_index = (xargs_config->number_of_command_args == 0)
		? 1 + args_buf_size
		: args_buf_size + xargs_config->number_of_command_args;
	}
    }
    

    if (args_buf_size == 0) return;
    
    
    args_buf[args_buf_index + 1] = NULL;
    
    queue_insert(
	&xargs_config->args,
	args_buf,
	args_buf_size);

    /* NOTE
     * The queue is responsible for freeing
     * args_buf!
     */
    args_buf = _args_buf_init(xargs_config, argv);
    args_buf_size = 0;
}


void xargs_free(config *xargs_config)
{
    if (xargs_config->command_needs_free)
	free(xargs_config->command);

    
    queue_free(&xargs_config->args);
}



#endif // __XARGS_H
