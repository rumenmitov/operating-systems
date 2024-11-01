#ifndef __ENV_H
#define __ENV_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

extern char **environ;

#define YELLOW "\033[33m"
#define RED    "\033[31m"
#define RESET  "\033[0m"


#define TRACE(TraceFlag, ...)				\
    if (TraceFlag) {					\
    	fprintf(stderr, "[" YELLOW "TRACE" RESET "] ");	\
    	fprintf(stderr, __VA_ARGS__);			\
    }


typedef enum EXIT_CODES {
    Sucess = 0,
    UnrecognizedOption,
    UnsetVariable,
    CommandExecution,
    SetVariable
} EXIT_CODES;

    
#define ERROR "[" RED "ERROR" RESET "]"


/**
 *@brief Parse options.
 *@param int argc
 *@param char *argv[]
 */
void parse_opts(int argc, char *argv[]);


/**
 *@brief Parse arguments (variables and commands).
 *@param int argc
 *@param char *argv[]
 */
void parse_args(int argc, char *argv[]);


/**
 *@brief Parse all environment variables.
 */
void env_print();
#endif // __ENV_H
