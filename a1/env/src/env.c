#include "env.h"

int TraceFlag = 0;

void parse_opts(int argc, char *argv[]) 
{
    opterr = 0;
    char opt;
    
    while ((opt = getopt(argc, argv, "+u:v")) != -1) {
	switch (opt) {
	case '?':
	    fprintf(stderr, ERROR ": Invalid option\n");
	    exit(UnrecognizedOption);

	case ':':
	    fprintf(stderr, ERROR ": Missing option\n");
	    exit(UnrecognizedOption);

	case 'u':
	    TRACE(TraceFlag, "Unsetting variable: %s\n", optarg);
	    if ( unsetenv(optarg) < 0 ) {
		perror(ERROR);
		exit(UnsetVariable);
	    }
	    break;

	case 'v':
	    TraceFlag = 1;
	    break;

	default:
	    break;
	    
	}
    }
}


void parse_args(int argc, char *argv[]) 
{
    for (unsigned int i = optind; i < argc; i++) {
	char *equals_sign_index = strchr(argv[i], '=');

	if (equals_sign_index == NULL) {
	    TRACE(TraceFlag, "Executing: %s\n", argv[i]);
	    if (execvp(argv[i], argv + i) < 0) {
		perror(ERROR);
		exit(CommandExecution);
	    }
	} else {
	    char name[1024];
	    char value[1024];

	    
	    strncpy(name, argv[i], equals_sign_index - argv[i]);
	    strncpy(value, equals_sign_index + 1, strlen(equals_sign_index));

	    TRACE(TraceFlag, "Setting variable: {name: %s, value: %s}\n", name, value);
	    if (setenv(name, value, 1) < 0) {
		perror(ERROR);
		exit(SetVariable);
	    }
	}
    }
}

void env_print() 
{
    TRACE(TraceFlag, "Printing environment\n");
    unsigned int i = 0;
    while (environ[i] != NULL)
	printf("%s\n", environ[i++]);
}
