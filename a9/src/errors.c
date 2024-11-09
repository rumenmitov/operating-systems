#include <stdlib.h>
#include <stdio.h>

#include "errors.h"

void die(char *e)
{
    perror(e);
    exit(EXIT_FAILURE);
}


void die2(char *e) 
{
    fprintf(stderr, "%s\n", e);
    exit(EXIT_FAILURE);
}

