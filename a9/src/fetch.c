#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "quiz.h"
#include "errors.h"

#define DEFAULT_BUF_SIZE 1024

static char CURL[]        = "curl";
static char CURLOPT[]     = "-s";


extern char* fetch(char *url) 
{
    // Create pipe
    int fds[2];
    if (pipe(fds) == -1) {
	die("pipe()");
    }

    // Spawn exec
    pid_t exec_pid = fork();

    if (exec_pid == 0) { /* child */
	if (close(fds[0]) == -1) {
	    die("close()");
	}

	if (dup2(fds[1], STDOUT_FILENO) == -1) {
	    die("dup2(), mapping stdout");
	}

	char *curl_args[] = { CURL, CURLOPT, url, NULL };
	
	execvp(curl_args[0], curl_args);
	die("execvp()");
	
    } else if (exec_pid > 0) { /* parent */
	struct stat curl_pipe_stats;
	int exec_status;

	if (waitpid(exec_pid, &exec_status, 0) == -1) {
	    die("waitpid()");
	}

	if (exec_status == EXIT_FAILURE) {
	    die("execvp()");
	}
	
	if (fstat(fds[0], &curl_pipe_stats) == -1) {
	    die("fstat()");
	}

	if (close(fds[1]) == -1) {
	    die("close()");
	}

	char *res = (char*) malloc(DEFAULT_BUF_SIZE);
	if (res == NULL) {
	    die("malloc()");
	}
	
	unsigned int res_cap = DEFAULT_BUF_SIZE;
	unsigned int res_size = 0;
	
	char ch;
	
	while (read(fds[0], (void*) &ch, 1) == 1) {
	    if (res_size == res_cap) {
		res_cap *= 2;
		res = (char*) realloc(res, res_cap);
		if (res == NULL) {
		    die("realloc()");
		}
	    }

	    res[res_size++] = ch;
	}

	if (close(fds[0]) == -1) {
	    die("close()");
	}

	if (res_size == res_cap) {
	    res_cap *= 2;
	    res = (char*) realloc(res, res_cap);
	    if (res == NULL) {
		die("realloc()");
	    }
	}

	res[res_size] = '\0';
	
	return res;
	
    } else {
	die("fork()");
    }
    
    return NULL;
}
