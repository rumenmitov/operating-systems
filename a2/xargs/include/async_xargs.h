/***
    async_xargs.h

    Provides a concurrent way to execute the
    given commands with their associated
    list of arguments.

    It works by first assigning the first n
    args to n workers. It then loops and waits for any
    worker to finish. When a worker finishes, he is
    assigned the next todo job.
*/


#ifndef __ASYNC_XARGS_H
#define __ASYNC_XARGS_H

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "xargs.h"
#include <err_xargs.h>


/**
 * @brief Contains information on how many concurrent
 *        threads should be running.
 * @field unsigned int workers
 * @field unsigned int tasks_done
 */
typedef struct arena 
{
    unsigned int workers;
    unsigned int tasks_done;
} arena;


/**
 * @brief Prints the running executable with its arguments.
 * @param char **argv
 * @param bool verbose
 */
static void _async_trace(char **argv, bool verbose)
{
    if (verbose == false) return;

    printf("%s", argv[0]);

    if (argv[1] != NULL) {
	for (int i = 1; argv[i] != NULL; i++) {
	    printf(" %s", argv[i]);
	}
    }

    printf("\n");
}


/**
 * @brief Forks and runs the command with the arguments.
 * @param char **argv
 */
static void _async_exec(char **argv, bool verbose)
{
    pid_t res = fork();
    if (res < 0) {
	perror(ERROR "fork()");
	exit(EXIT_FAILURE);
    }

    if (res == 0) {
	_async_trace(argv, verbose);
	if (execvp(argv[0], argv) == -1) {
	    perror(ERROR "execvp()");
	    exit(EXIT_FAILURE);
	}
    }
}
    

/**
 * @brief Executes the commands concurrently
 *        (the number of jobs is configured through
 *        xargs_config).
 * @param arena *arena_config
 * @param config *xargs_config
 */
void async_xarg_exec(arena *arena_config, config *xargs_config)
{
    for (int i = 0; i < arena_config->workers; i++) {
	
	_async_exec(xargs_config->args._container[i],
		    xargs_config->verbose);
    }
    
    while (arena_config->tasks_done < xargs_config->args.size) {
	if ( waitpid(-1, NULL, 0) == -1 ) {
	    perror(ERROR "waitpid()");
	    exit(EXIT_FAILURE);
	}

	unsigned int next_task_index = arena_config->tasks_done + arena_config->workers;
	arena_config->tasks_done++;

	if (next_task_index < xargs_config->args.size) {
	    _async_exec(xargs_config->args._container[next_task_index],
			xargs_config->verbose);
	}
    }
}
    


#endif // __ASYNC_XARGS_H
