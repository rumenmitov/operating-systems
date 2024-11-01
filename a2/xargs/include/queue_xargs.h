/***
    queue_xargs.h

    Contains a basic queue implementation for storing
    arrays of strings.

*/


#ifndef __QUEUE_XARGS_H
#define __QUEUE_XARGS_H

#include <sys/types.h>


/**
 * @brief Technically a stack, I consider this a queue implementation
 *        because when I run the commands in async, they are called
 *        in a FIFO manner.
 * @field unsigned int size
 * @field unsigned int _cap
 * @field char ***_container
 * @field unsigned int *_container_elements_size
 */
typedef struct queue 
{
    unsigned int size;
    unsigned int _cap;
    
    char ***_container;
    unsigned int *_container_elements_sizes;
    
} queue;

/**
 * @brief Initializes queue with a default capacity of 5.
 * @param queue *q
 */
void queue_init(queue *q);

/**
 * @brief Inserts an array of strings into the queue. The queue
 *        is repsponsible for freeing this as well, hence we
 *        need to keep track of how many elements it contains.
 * @param queue *q
 * @param char **payload
 * @param int payload_size
 */
void queue_insert(queue *q, char **payload, unsigned int payload_size);

/**
 * @brief Frees all elements (sub-elements included).
 * @param queue *q
 */
void queue_free(queue *q);



#endif // __QUEUE_XARGS_H
