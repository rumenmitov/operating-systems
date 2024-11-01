/***
    pdi.h

    Functions for calculating the Perfect Digital
    Invariant numbers in a given range concurrently.
*/


#ifndef __PDI_H
#define __PDI_H

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#define RED "\033[31m"
#define RESET "\033[0m"

#define ERROR "[" RED "ERROR" RESET "]: "


/**
 * @brief Defines upper and lower bound.
 */
typedef struct bounds 
{
    uint64_t lower;
    uint64_t upper;
} bounds;
    

/**
 * @brief Main config for managing state of the program.
 */
typedef struct pdi_config 
{
    bounds bounds;
    uint32_t threads;
    bool verbose;
} pdi_config;


/**
 * @brief Sane defaults for initializing the config.
 */
typedef enum pdi_defaults
{
    lower = 1,
    upper = 10000,

    threads = 1,

    verbose = false
} pdi_defualts;


/**
 * @brief Struct for managing threads that look for pdi numbers.
 */
typedef struct thread_config 
{
    pthread_t thread;
    bounds bounds;
    
} thread_config;


/**
 * @brief Parses options.
 */
static void _parse_opts(pdi_config *config, int argc, char *argv[]) 
{
    opterr = 0;
    char opt;

    while ((opt = getopt(argc, argv, "+s:e:t:v")) != -1) {
	switch (opt) {
	case '?':
	    fprintf(stderr, ERROR "Unknown option!\n");
	    exit(EXIT_FAILURE);
	    break;

	case ':':
	    fprintf(stderr, ERROR "Missing argument!\n");
	    exit(EXIT_FAILURE);	    
	    break;

	case 's':
	    config->bounds.lower = atol(optarg);
	    if (config->bounds.lower <= 0) {
		fprintf(
		    stderr,
		    ERROR "Lower bound must be a positive non-zero integer!\n");
		exit(EXIT_FAILURE);
	    }
		
	    break;

	case 'e':
	    config->bounds.upper = atol(optarg);
	    if (config->bounds.upper <= 0) {
		fprintf(
		    stderr,
		    ERROR "Upper bound must be a positive non-zero integer!\n");
		exit(EXIT_FAILURE);
	    }
	    
	    break;

	case 't':
	    config->threads = atol(optarg);
	    if (config->threads <= 0) {
		fprintf(
		    stderr,
		    ERROR "Threads must be a positive non-zero integer!\n");
		exit(EXIT_FAILURE);
	    }
		
	    break;

	case 'v':
	    config->verbose = true;
	    break;

	default:
	    break;
	}

	if (config->bounds.lower > config->bounds.upper) {
	    fprintf(stderr, ERROR "Lower bound cannot be greater than upper bound!\n");
	    exit(EXIT_FAILURE);
	}

	if (config->threads > config->bounds.upper - config->bounds.lower) {
	    config->threads = config->bounds.upper - config->bounds.lower;
	}
	
    }
}


/**
 * @brief Initialized pdi config.
 */
void pdi_init(pdi_config *config, int argc, char *argv[]) 
{
    config->bounds.lower = lower;
    config->bounds.upper = upper;
    config->threads = threads;
    config->verbose = verbose;
    
    _parse_opts(config, argc, argv);
}


/**
 * @brief Checks if number is pdi.
 */
static bool _is_pdi(uint64_t num) 
{
    /* NOTE
     * The biggest possible uint64_t is:
     * 18446744073709551616
     * which has 20 digits.
     */
    char buf[20];
    memset(buf, 0, 20);

    snprintf(buf, 20, "%lu", num);

    for (uint64_t p = 0;; p++) {
	uint64_t sum = 0;

	for (int i = 0; i < strlen(buf); i++) {
	    unsigned int digit = buf[i] - '0';

	    sum += pow(digit, p);
	}


	if (sum == num) return true;
	else if (sum > num || p > num) return false; // we overshot num
	else continue;
    }
}


/**
 * @brief Prints all the pdi numbers in a given range. Called from thread.
 */
static void *_print_pdi(void *thread)
{
    thread_config *me = (thread_config *) thread;

    for (uint64_t i = me->bounds.lower; i <= me->bounds.upper; i++) {
	if (_is_pdi(i)) printf("%lu\n", i);
    }
    
    
    return NULL;
}


/**
 * @brief Helper function to calculate the amount of numbers a thread should go
 *        through.
 */
static uint64_t _range_per_thread(const bounds *bounds, uint32_t total_threads) 
{
    uint64_t range = bounds->upper - bounds->lower + 1; // we count the upper bound
    return range / total_threads;
}


/**
 * @brief Searches for pdi numbers concurrently.
 */
void pdi(const pdi_config *config) 
{
    thread_config tids[config->threads];
    const uint64_t range_per_thread = _range_per_thread(&config->bounds, config->threads);

    for (uint32_t i = 0; i < config->threads; i++) {
	tids[i].bounds.lower = config->bounds.lower + i * range_per_thread;
	tids[i].bounds.upper = tids[i].bounds.lower + range_per_thread - 1;

	/* NOTE
	 * In some cases the last upper boundary is less than
	 * it should be (e.g. 10000 / 3 = 9999). This prevents
	 * this.
	 */
	if (i == config->threads - 1 &&
	    tids[i].bounds.upper != config->bounds.upper) {

	    tids[i].bounds.upper = config->bounds.upper;
	}
	

	if (config->verbose) {
	    printf(
		"pdi-numbers: t%u searching [%lu,%lu]\n",
		i,
		tids[i].bounds.lower,
		tids[i].bounds.upper);
	}
	

	if (pthread_create(&tids[i].thread, NULL, _print_pdi, &tids[i]) != 0) {
	    fprintf(stderr, ERROR "Failed to create thread!\n");
	    exit(EXIT_FAILURE);
	}
    }

    for (uint32_t i = 0; i < config->threads; i++) {
	if (pthread_join(tids[i].thread, NULL) != 0) {
	    fprintf(stderr, ERROR "Failed to join thread!\n");
	    exit(EXIT_FAILURE);
	}

	if (config->verbose) {
	    printf("pdi-numbers: t%u finishing\n", i);
	}
    }
}


#endif // __PDI_H
