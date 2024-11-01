#include <stdlib.h>
#include <stdio.h>

#include "sims.h"


void simulation_init(config *config) 
{
    for (uint8_t i = 0; i < PRISONERS; i++) {
	config->prisoners[i].drawers_viewed = DrawersViewed;
	config->prisoners[i].found          = Found;
	
	if (pthread_mutex_init(&drawers[i].lock, NULL) != 0) {
	    fprintf(stderr, ERROR "Couldn't create mutex!\n");
	    exit(EXIT_FAILURE);
	}

	/* NOTE
	 * Generate a random number from 1 to PRISONERS,
	 * make sure it is not already taken.
	 */
    REGEN:
	drawers[i].payload = (rand() % PRISONERS) + 1;
	for (uint8_t j = 0; j < i; j++) {
	    if (drawers[i].payload == drawers[j].payload)
		goto REGEN;
	}
	

	if (pthread_mutex_init(&drawers[i].lock, NULL) != 0) {
	    fprintf(stderr, ERROR "Couldn't create mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }
}


static void* _sim_random_drawer(void *_prisoner) 
{
    prisoner *sim_prisoner = (prisoner*) _prisoner;

    uint8_t attempts = 0;
    uint8_t guess    = 0;
    
    while (attempts < MAX_DRAWERS && sim_prisoner->found == false) {
	guess = rand() % PRISONERS;

	if (pthread_mutex_lock(&drawers[guess].lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	if (sim_prisoner->number == drawers[guess].payload)
	    sim_prisoner->found = true;
	else attempts++;

	if (pthread_mutex_unlock(&drawers[guess].lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }

    return NULL;
}


static void* _sim_smart_drawer(void *_prisoner) 
{
    prisoner *sim_prisoner = (prisoner*) _prisoner;

    uint8_t attempts  = 0;
    uint8_t guess     = sim_prisoner->number;
    uint8_t old_guess = sim_prisoner->number;
    
    while (attempts < MAX_DRAWERS && sim_prisoner->found == false) {
	old_guess = guess;
	
	if (pthread_mutex_lock(&drawers[guess].lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	if (sim_prisoner->number == drawers[guess].payload)
	    sim_prisoner->found = true;
	else {
	    attempts++;
	    guess = drawers[guess].payload;
	}

	if (pthread_mutex_unlock(&drawers[old_guess].lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }


    return NULL;
}


static void* _sim_random_global(void *_prisoner) 
{
    prisoner *sim_prisoner = (prisoner*) _prisoner;

    uint8_t attempts = 0;
    uint8_t guess    = 0;
    
    while (attempts < MAX_DRAWERS && sim_prisoner->found == false) {
	guess = rand() % PRISONERS;

	if (pthread_mutex_lock(&global_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	if (sim_prisoner->number == drawers[guess].payload)
	    sim_prisoner->found = true;
	else attempts++;

	if (pthread_mutex_unlock(&global_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }

    return NULL;
}


static void* _sim_smart_global(void *_prisoner) 
{
    prisoner *sim_prisoner = (prisoner*) _prisoner;

    uint8_t attempts = 0;
    uint8_t guess    = sim_prisoner->number;
    
    while (attempts < MAX_DRAWERS && sim_prisoner->found == false) {
	if (pthread_mutex_lock(&global_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't lock mutex!\n");
	    exit(EXIT_FAILURE);
	}

	if (sim_prisoner->number == drawers[guess].payload)
	    sim_prisoner->found = true;
	else {
	    attempts++;
	    guess = drawers[guess].payload;
	}

	if (pthread_mutex_unlock(&global_lock) != 0) {
	    fprintf(stderr, ERROR "Couldn't unlock mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }

    return NULL;
}


enum GameResult run_sim_random_drawer(config *config)
{
    for (int i = 0; i < PRISONERS; i++) {
	if (pthread_create(
		&config->prisoners[i].thread,
		NULL,
		_sim_random_drawer,
		&config->prisoners[i]) != 0) {

	    fprintf(stderr, ERROR "Couldn't create thread!\n");
	    exit(EXIT_FAILURE);
	}
    }

    enum GameResult res = Win;
    
    for (uint8_t i = 0; i < PRISONERS; i++) {
	if (pthread_join(config->prisoners[i].thread, NULL) != 0) {
	    fprintf(stderr, ERROR "Couldn't join thread!\n");
	    exit(EXIT_FAILURE);
	}

	if (config->prisoners[i].found == false) res = Loss;
    }

    return res;
}


enum GameResult run_sim_smart_drawer(config *config)
{
    for (uint8_t i = 0; i < PRISONERS; i++) {
	if (pthread_create(
		&config->prisoners[i].thread,
		NULL,
		_sim_smart_drawer,
		&config->prisoners[i]) != 0) {

	    fprintf(stderr, ERROR "Couldn't create thread!\n");
	    exit(EXIT_FAILURE);
	}
    }

    enum GameResult res = Win; 
    
    for (uint8_t i = 0; i < PRISONERS; i++) {
	if (pthread_join(config->prisoners[i].thread, NULL) != 0) {
	    fprintf(stderr, ERROR "Couldn't join thread!\n");
	    exit(EXIT_FAILURE);
	}

	if (config->prisoners[i].found == false) res = Loss;
    }


    return res;
}


enum GameResult run_sim_random_global(config *config)
{
    for (uint8_t i = 0; i < PRISONERS; i++) {
	if (pthread_create(
		&config->prisoners[i].thread,
		NULL,
		_sim_random_global,
		&config->prisoners[i]) != 0) {

	    fprintf(stderr, ERROR "Couldn't create thread!\n");
	    exit(EXIT_FAILURE);
	}
    }

    enum GameResult res = Win;
    
    for (uint8_t i = 0; i < PRISONERS; i++) {
	if (pthread_join(config->prisoners[i].thread, NULL) != 0) {
	    fprintf(stderr, ERROR "Couldn't join thread!\n");
	    exit(EXIT_FAILURE);
	}

	if (config->prisoners[i].found == false) res = Loss;
    }

    return res;
}


enum GameResult run_sim_smart_global(config *config)
{
    for (uint8_t i = 0; i < PRISONERS; i++) {
	if (pthread_create(
		&config->prisoners[i].thread,
		NULL,
		_sim_smart_global,
		&config->prisoners[i]) != 0) {

	    fprintf(stderr, ERROR "Couldn't create thread!\n");
	    exit(EXIT_FAILURE);
	}
    }

    enum GameResult res = Win;
    
    for (uint8_t i = 0; i < PRISONERS; i++) {
	if (pthread_join(config->prisoners[i].thread, NULL) != 0) {
	    fprintf(stderr, ERROR "Couldn't join thread!\n");
	    exit(EXIT_FAILURE);
	}

	if (config->prisoners[i].found == false) res = Loss;
    }

    return res;
}

