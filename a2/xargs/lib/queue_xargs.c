#include <stdlib.h>
#include <stdio.h>

#include "queue_xargs.h"
#include "err_xargs.h"


void queue_init(queue *q)
{
    q->_cap = 5;
    q->size = 0;

    q->_container =
	(char***) malloc( sizeof(char**) * q->_cap );

    if (q->_container == NULL) {
	perror(ERROR "malloc()");
	exit(EXIT_FAILURE);
    }

    q->_container_elements_sizes =
	(unsigned int*) malloc( sizeof(unsigned int) * q->_cap );
    
    if (q->_container_elements_sizes == NULL) {
	perror(ERROR "malloc()");
	exit(EXIT_FAILURE);
    }
    
}


void queue_insert(queue *q, char **payload, unsigned int payload_size) 
{
    if (q->size == q->_cap - 1) {
	q->_cap *= 2;

	q->_container =
	    (char***) realloc( q->_container, sizeof(char**) * q->_cap );
	
	if (q->_container == NULL) {
	    perror(ERROR "realloc()");
	    exit(EXIT_FAILURE);
	}


	q->_container_elements_sizes =
	    (unsigned int*) realloc( q->_container_elements_sizes, sizeof(unsigned int) * q->_cap );
	
	if (q->_container_elements_sizes == NULL) {
	    perror(ERROR "realloc()");
	    exit(EXIT_FAILURE);
	}

    }

    q->_container[q->size] = payload;
    q->_container_elements_sizes[q->size++] = payload_size;
}


void queue_free(queue *q) 
{
    /* NOTE
     * We do not want to double free the command!
     */
    for (int i = 0; i < q->size; i++) {
	for (int j = 1; j < q->_container_elements_sizes[i]; j++) {
	    free(q->_container[i][j]);
	    q->_container[i][j] = NULL;
	}
	
	free(q->_container[i]);
	q->_container[i] = NULL;
    }
    

    free(q->_container);
    q->_container = NULL;

    free(q->_container_elements_sizes);
    q->_container_elements_sizes = NULL;
}
