#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


#include "prisoner.h"
#include "sims.h"

drawer drawers[];
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;


void config_init(int argc, char *argv[], config *config) 
{
    config->games    = Games;
    config->seed     = Seed;
    config->seed_set = SeedSet;

    for (uint8_t i = 0; i < PRISONERS; i++) {
	config->prisoners[i].number         = i;
	config->prisoners[i].drawers_viewed = DrawersViewed;
	config->prisoners[i].found          = Found;
    }
    

    opterr = 0;

    char opt;
    while ((opt = getopt(argc, argv, "+n:s:")) != -1) {
	switch (opt) {
	case '?':
	    fprintf(stderr, ERROR "Invalid option!\n");
	    exit(EXIT_FAILURE);

	case ':':
	    fprintf(stderr, ERROR "Missing arugment!\n");
	    exit(EXIT_FAILURE);

	case 'n':
	    config->games = strtol(optarg, NULL, 10);
	    if (config->games == LONG_MAX || config->games == LONG_MIN) {
		perror(ERROR "strtol()");
		exit(EXIT_FAILURE);
	    }

	    if (config->games <= 0) {
		fprintf(stderr, "Games must be a positive integer!\n");
		exit(EXIT_FAILURE);
	    }
	    
	    break;

	case 's':
	    config->seed = strtol(optarg, NULL, 10);
	    if (config->seed == LONG_MAX || config->seed == LONG_MIN) {
		perror(ERROR "strtol()");
		exit(EXIT_FAILURE);
	    }

	    if (config->seed < 0) {
		fprintf(stderr, ERROR "Seed must be a non-negative integer!\n");
		exit(EXIT_FAILURE);
	    }
	    

	    config->seed_set = true;
	    break;

	default:
	    break;
	}
    }

    if (config->seed_set) srand(config->seed);
    else srand(time(NULL));
}


static stats
_get_stats(enum GameResult (*sim)(config*), config *config) 
{
    stats res = STATS_INIT;
    
    clock_t t1, t2;
    t1 = clock();

    for (uint64_t i = 0; i < config->games; i++) {
	simulation_init(config);
	if (sim(config) == Win)
	    res.wins++;
    }

    t2 = clock();
    
    res.time = ((double) t2 - (double) t1) / CLOCKS_PER_SEC * 1000;
    return res;
}



void run(config *config) 
{
    stats random_global_stats = _get_stats(run_sim_random_global, config);
    stats random_drawer_stats = _get_stats(run_sim_random_drawer, config);
    stats smart_global_stats  = _get_stats(run_sim_smart_global, config);
    stats smart_drawer_stats  = _get_stats(run_sim_smart_drawer, config);

    // random_global
    printf("random_global %5li/%li wins/games = %8.2lf%% %8.3lf/%li ms = %5.3lf ms\n",
	   random_global_stats.wins,
	   config->games,
	   (double) random_global_stats.wins * 100 / config->games,
	   random_global_stats.time,
	   config->games,
	   random_global_stats.time / config->games
	);

    // random_drawer
    printf("random_drawer %5li/%li wins/games = %8.2lf%% %8.3lf/%li ms = %5.3lf ms\n",
	   random_drawer_stats.wins,
	   config->games,
	   (double) random_drawer_stats.wins * 100 / config->games,
	   random_drawer_stats.time,
	   config->games,
	   random_drawer_stats.time / config->games
	);

    // smart global
     printf("smart_global %6li/%li wins/games = %8.2lf%% %8.3lf/%li ms = %5.3lf ms\n",
	   smart_global_stats.wins,
	   config->games,
	   (double) smart_global_stats.wins * 100 / config->games,
	   smart_global_stats.time,
	   config->games,
	   smart_global_stats.time / config->games
	);

    // smart drawer
    printf("smart_drawer %6li/%li wins/games = %8.2lf%% %8.3lf/%li ms = %5.3lf ms\n",
	   smart_drawer_stats.wins,
	   config->games,
	   (double) smart_drawer_stats.wins * 100 / config->games,
	   smart_drawer_stats.time,
	   config->games,
	   smart_drawer_stats.time / config->games
	);
}



void quit(config *config) 
{
    for (int i = 0; i < PRISONERS; i++) {
	if (pthread_mutex_destroy(&drawers[i].lock) != 0) {
	    fprintf(stderr, "Couldn't destroy mutex!\n");
	    exit(EXIT_FAILURE);
	}
    }
}
