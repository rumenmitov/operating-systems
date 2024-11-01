/***
    prisoner.h

    Contains initialization, timing of the execution and the cleanup.
*/

#ifndef __PRISONER_H
#define __PRISONER_H

#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>

/**
 * @brief Error pretty-printing.
 */
#define RED   "\033[31m"
#define RESET "\033[0m"

#define ERROR "[" RED "ERROR" RESET "]: "

/**
 * @brief Prisoners and max amount of drawers a prisoner can open do not change.
 */
#define PRISONERS   100
#define MAX_DRAWERS 50

/**
 * @brief Drawer that contains a random prisoner's number and a lock.
 */
typedef struct drawer 
{
    uint8_t payload;
    pthread_mutex_t lock;
} drawer;

/**
 * @brief All the drawers.
 */
extern drawer drawers[PRISONERS];

/**
 * @brief Global lock for running synchronosly.
 */
extern pthread_mutex_t global_lock;

/**
 * @brief Keeps track of prisoner thread and its associated information.
 */
typedef struct prisoner 
{
    uint8_t number;
    uint8_t drawers_viewed;
    bool found;

    drawer *drawers;

    pthread_t thread;
    
} prisoner;

/**
 * @brief For initializing / resetting prisoner properties.
 */
enum PrisonerDefaults {
    DrawersViewed = 0,
    Found         = false
};
    
/**
 * @brief Contains program information.
 */
typedef struct config 
{
    int64_t  games;
    int64_t  seed;
    bool     seed_set;

    prisoner prisoners[PRISONERS];
} config;

/**
 * @brief For initializing program config.
 */
enum ConfigDefaults
{
    Games   = 100,
    Seed    = 0,
    SeedSet = false
};

/**
 * @brief Possible outcomes of a game.
 */
enum GameResult
{
    Win,
    Loss
};

/**
 * @brief Stats for a given game.
 */
typedef struct stats 
{
    int64_t wins;
    double   time;
} stats;

/**
 * @brief Initializes a stats struct.
 */
#define STATS_INIT                                                           \
  { .wins = 0, .time = 0.00 }

/**
 * @brief Initializes config.
 */
void config_init(int argc, char *argv[], config *config);

/**
 * @brief Runs the program and prints results.
 */
void run(config *config);

/**
 * @brief Cleans up program.
 */
void quit(config *config);

#endif // __PRISONER_H
