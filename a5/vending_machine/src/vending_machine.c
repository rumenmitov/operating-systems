#include <stdio.h>
#include <getopt.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "vending_machine.h"


#define TRACE_DRINKS    1
#define TRACE_COINS     1 >> 1
#define TRACE_INSERTED  1 >> 2
#define TRACE_ALL       TRACE_DRINKS | TRACE_COINS | TRACE_INSERTED


static void _trace(vending_machine_config *config, const char *mes, uint8_t flags) 
{
    if (flags & TRACE_DRINKS) {
	if (pthread_mutex_lock(&config->drinks_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }
    

    if (flags & TRACE_COINS) {
	if (pthread_mutex_lock(&config->coins_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }
    
    if (flags & TRACE_INSERTED) {
	if (pthread_mutex_lock(&config->coins_inserted_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }
    
    printf("energy: [%lu/%lu drinks, %lu coins, %lu inserted] %s",
	   config->drinks,
	   config->capacity,
	   config->coins,
	   config->coins_inserted,
	   mes
	);

    if (flags & TRACE_INSERTED) {
	if (pthread_mutex_unlock(&config->coins_inserted_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }

    if (flags & TRACE_COINS) {
	if (pthread_mutex_unlock(&config->coins_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }

    if (flags & TRACE_DRINKS) {
	if (pthread_mutex_unlock(&config->drinks_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }
}


void* student(void* student_args) 
{
    student_t *student_config = (student_t*) student_args;
    vending_machine_config *config = student_config->config;

    char message[1024];

    if (pthread_mutex_lock(&config->student_lock) != 0) {
	fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	exit(EXIT_FAILURE);
    }

    snprintf(message, 1024, "student %lu requires an energy drink\n", student_config->id);
    _trace(config, message, TRACE_ALL);

    
    // drinks in machine
    if (pthread_mutex_lock(&config->drinks_lock) != 0) {
	fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	exit(EXIT_FAILURE);
    }

    while (config->drinks == 0) {	
	snprintf(message, 1024, "student %lu waiting for machine to be refilled\n", student_config->id);
	_trace(config, message, TRACE_COINS | TRACE_INSERTED);
	
	if (pthread_cond_wait(&config->drinks_cond, &config->drinks_lock) != 0) {
	    fprintf(stderr, ERROR "Conditional variable wait failed!\n");
	    exit(EXIT_FAILURE);
	}
    }

    if (pthread_mutex_unlock(&config->drinks_lock) != 0) {
	fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	exit(EXIT_FAILURE);
    }


    if (pthread_mutex_lock(&config->coins_inserted_lock) != 0) {
	fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	exit(EXIT_FAILURE);
    }

    while (config->coins_inserted < DRINK_COST) {
	config->coins_inserted++;

	snprintf(message, 1024, "student %lu inserted another coin\n", student_config->id);
	_trace(config, message, TRACE_DRINKS | TRACE_COINS);
    }

    if (pthread_mutex_unlock(&config->coins_inserted_lock) != 0) {
	fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_cond_signal(&config->coins_inserted_cond) != 0) {
	fprintf(stderr, ERROR "Conditional variable signal failed!\n");
	exit(EXIT_FAILURE);
    }

    snprintf(message, 1024, "student %lu waiting for drink to arrive\n", student_config->id);
    _trace(config, message, TRACE_ALL);

    if (pthread_mutex_lock(&config->drink_dispensed_lock) != 0) {
	fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	exit(EXIT_FAILURE);
    }

    while (config->drink_dispensed == false) {
	if (pthread_cond_wait(&config->drink_dispensed_cond, &config->drink_dispensed_lock) != 0) {
	    fprintf(stderr, ERROR "Conditional variable wait failed!\n");
	    exit(EXIT_FAILURE);
	}
    }

    config->drink_dispensed = false;

    if (pthread_mutex_unlock(&config->drink_dispensed_lock) != 0) {
	fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	exit(EXIT_FAILURE);
    }

    snprintf(message, 1024, "student %lu picked up a drink\n", student_config->id);
    _trace(config, message, TRACE_ALL);

    if (pthread_cond_signal(&config->drink_dispensed_cond) != 0) {
	fprintf(stderr, ERROR "Conditional variable signal failed!\n");
	exit(EXIT_FAILURE);
    }
    
    if (pthread_mutex_unlock(&config->student_lock) != 0) {
	fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	exit(EXIT_FAILURE);
    }

    snprintf(message, 1024, "student %lu enjoying an energy drink\n", student_config->id);
    _trace(config, message, TRACE_ALL);
    
    return NULL;
}


void* machine(void *glob_config) 
{
    vending_machine_config *config = (vending_machine_config*) glob_config;
    
    while (true) {
	// coins inserted
	if (pthread_mutex_lock(&config->coins_inserted_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	while (config->coins_inserted < DRINK_COST) {
	    _trace(config, "machine waiting for more coins\n", TRACE_DRINKS | TRACE_COINS);

	    if (pthread_cond_wait(&config->coins_inserted_cond, &config->coins_inserted_lock) != 0) {
		fprintf(stderr, ERROR "Conditional variable wait failed!\n");
		exit(EXIT_FAILURE);
	    }
	}

	config->coins_inserted = 0;

	if (pthread_mutex_unlock(&config->coins_inserted_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	// total coins
	if (pthread_mutex_lock(&config->coins_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	config->coins += DRINK_COST;

	if (pthread_mutex_unlock(&config->coins_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	// dispense drink

	if (pthread_mutex_lock(&config->drinks_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	config->drinks--;
	
	if (pthread_mutex_unlock(&config->drinks_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	_trace(config, "machine dispensing drink\n", TRACE_ALL);

	if (pthread_mutex_lock(&config->drink_dispensed_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	config->drink_dispensed = true;
	
	if (pthread_mutex_unlock(&config->drink_dispensed_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	if (pthread_cond_signal(&config->drink_dispensed_cond) != 0) {
	    fprintf(stderr, ERROR "Conditional variable signal failed!\n");
	    exit(EXIT_FAILURE);
	}

	if (pthread_mutex_lock(&config->drink_dispensed_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	while (config->drink_dispensed == true) {
	    _trace(config, "machine waiting for pickup of drink\n", TRACE_ALL);

	    if (pthread_cond_wait(&config->drink_dispensed_cond, &config->drink_dispensed_lock) != 0) {
		fprintf(stderr, ERROR "Conditional variable wait failed!\n");
		exit(EXIT_FAILURE);
	    }
	}

	config->drink_dispensed = DrinkDispensed;

	if (pthread_mutex_unlock(&config->drink_dispensed_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }

    return NULL;
}


void* supplier(void* glob_config) 
{
    vending_machine_config *config = (vending_machine_config*) glob_config;
    char message[1024];

    /** NOTE
     * Supplier comes every day, hence the loop with the sleep function.
     */
    while (true) {
	_trace(config, "supplier arriving\n", TRACE_ALL);

	if (pthread_mutex_lock(&config->drinks_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	snprintf(message, 1024, "supplier loading %lu drinks\n", config->capacity - config->drinks);
	_trace(config, message, TRACE_COINS | TRACE_INSERTED);

	config->drinks = config->capacity;
    
	if (pthread_mutex_unlock(&config->drinks_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	if (pthread_mutex_lock(&config->coins_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	snprintf(message, 1024, "supplier collected %lu coins\n", config->coins);
	config->coins = 0;
	_trace(config, message, TRACE_DRINKS | TRACE_INSERTED);

    
	if (pthread_mutex_unlock(&config->coins_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	if (pthread_cond_signal(&config->drinks_cond) != 0) {
	    fprintf(stderr, ERROR "Conditional variable signal failed!\n");
	    exit(EXIT_FAILURE);
	}

	_trace(config, "supplier leaving\n", TRACE_ALL);

	sleep(SUPPLIER_INTERVAL);
    }
    
    return NULL;
}



void config_init(int argc, char *argv[], vending_machine_config *config) 
{
    config->students        = Students;
    config->capacity        = Capacity;
    config->coins_inserted  = CoinsInserted;
    config->drinks          = Drinks;
    config->coins           = Coins;
    config->drink_dispensed = DrinkDispensed;
   

    if (pthread_mutex_init(&config->student_lock, 0) != 0) {
	fprintf(stderr, ERROR "Failed to init mutex!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&config->drinks_cond, 0) != 0) {
	fprintf(stderr, ERROR "Failed to init conditional variable!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&config->drinks_lock, 0) != 0) {
	fprintf(stderr, ERROR "Failed to init mutex!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&config->coins_inserted_cond, 0) != 0) {
	fprintf(stderr, ERROR "Failed to init conditional variable!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&config->coins_inserted_lock, 0) != 0) {
	fprintf(stderr, ERROR "Failed to init mutex!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&config->coins_lock, 0) != 0) {
	fprintf(stderr, ERROR "Failed to init mutex!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&config->drink_dispensed_cond, 0) != 0) {
	fprintf(stderr, ERROR "Failed to init conditional variable!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&config->drink_dispensed_lock, 0) != 0) {
	fprintf(stderr, ERROR "Failed to init mutex!\n");
	exit(EXIT_FAILURE);
    }
    
    
    opterr = 0;

    char opt = 0;
    while ((opt = getopt(argc, argv, "+n:c:")) != -1) {
	switch (opt) {
	case '?':
	    fprintf(stderr, ERROR "Unknown option or missing argument!\n");
	    exit(EXIT_FAILURE);

	case 'n':	    
	    config->students = atol(optarg);
	    if (config->students <= 0) {
		fprintf(stderr, ERROR "Number of students must be a positive, non-zero integer.\n");		
		exit(EXIT_FAILURE);
	    }
	    break;

	case 'c':
	    config->capacity = atol(optarg);
	    if (config->capacity < 0) {
		fprintf(stderr, ERROR "Capacity must be a positive, non-zero integer.\n");				
		exit(EXIT_FAILURE);
	    }
	    break;

	default:
	    break;
	}
    }
}


void run(vending_machine_config *config) 
{
    student_t student_threads[config->students];
    pthread_t machine_thread;
    pthread_t supplier_thread;

    char message[1024];

    for (int64_t i = 0; i < config->students; i++) {
	student_threads[i].id = i;
	student_threads[i].config = config;

	if (pthread_create(&student_threads[i].thread, NULL, student, (void*) &student_threads[i]) != 0) {
	    fprintf(stderr, ERROR "Failed to spawn thread!\n");
	    exit(EXIT_FAILURE);
	}
	snprintf(message, 1024, "student %lu established\n", i);
	_trace(config, message, TRACE_ALL);
    }

    if (pthread_create(&supplier_thread, NULL, supplier, (void*) config) != 0) {
	fprintf(stderr, ERROR "Failed to spawn thread!\n");
	exit(EXIT_FAILURE);
    }
    _trace(config, "supplier established\n", TRACE_ALL);


    _trace(config, "machine booting up\n", TRACE_ALL);
    if (pthread_create(&machine_thread, NULL, machine, (void*) config) != 0) {
	fprintf(stderr, ERROR "Failed to spawn thread!\n");
	exit(EXIT_FAILURE);
    }

    
    for (int64_t i = 0; i < config->students; i++) {
	if (pthread_join(student_threads[i].thread, NULL) != 0) {
	    fprintf(stderr, ERROR "Failed to join thread!\n");
	    exit(EXIT_FAILURE);
	}
    }

    /** NOTE
     * No need to wait for vending machine or supplier to end,
     * since they will go on forever.
     */
}


void quit(vending_machine_config* config) 
{
    if (pthread_mutex_destroy(&config->student_lock) != 0) {
	fprintf(stderr, ERROR "Failed to destory mutex!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_cond_destroy(&config->drinks_cond) != 0) {
	fprintf(stderr, ERROR "Failed to destory conditional variable!\n");
	exit(EXIT_FAILURE);
    }
    
    if (pthread_mutex_destroy(&config->drinks_lock) != 0) {
	fprintf(stderr, ERROR "Failed to destory mutex!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_cond_destroy(&config->coins_inserted_cond) != 0) {
	fprintf(stderr, ERROR "Failed to destory conditional variable!\n");
	exit(EXIT_FAILURE);
    }
    
    if (pthread_mutex_destroy(&config->coins_inserted_lock) != 0) {
	fprintf(stderr, ERROR "Failed to destory mutex!\n");
	exit(EXIT_FAILURE);
    }

    if (pthread_mutex_destroy(&config->coins_lock) != 0) {
	fprintf(stderr, ERROR "Failed to destory mutex!\n");
	exit(EXIT_FAILURE);
    }
    
    if (pthread_mutex_destroy(&config->drink_dispensed_lock) != 0) {
	fprintf(stderr, ERROR "Failed to destory mutex!\n");
	exit(EXIT_FAILURE);
    }
    
    if (pthread_cond_destroy(&config->drink_dispensed_cond) != 0) {
	fprintf(stderr, ERROR "Failed to destory conditional variable!\n");
	exit(EXIT_FAILURE);
    }

}
