/***
    sims.h

    Contains functions for running the different scenarios.
*/


#ifndef __SIMS_H
#define __SIMS_H

#include "prisoner.h"

/**
 * @brief Initializes / resets variables needed for simulation (drawers, locks, etc.).
 */
void simulation_init(config *config);

/**
 * @brief Runs the random strategy with a per-drawer lock.
 */
enum GameResult run_sim_random_drawer(config *config);

/**
 * @brief Runs the smart strategy with a per-drawer lock.
 */
enum GameResult run_sim_smart_drawer(config *config);

/**
 * @brief Runs the random strategy with a global lock.
 */
enum GameResult run_sim_random_global(config *config);

/**
 * @brief Runs the smart strategy with a global lock.
 */
enum GameResult run_sim_smart_global(config *config);


#endif // __SIMS_H
