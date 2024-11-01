/***
    vending_machine.h
*/

#ifndef __VENDING_MACHINE_H
#define __VENDING_MACHINE_H

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
 * @brief Constants from assignment.
 */
#define DRINK_COST 5
#define SUPPLIER_INTERVAL 1


/**
 * @brief Stores shared data and associated locks.
 */
typedef struct vending_machine_config 
{
    int64_t students;
    int64_t capacity;

    pthread_mutex_t student_lock;

    int64_t drinks;
    pthread_cond_t  drinks_cond;
    pthread_mutex_t drinks_lock;

    int64_t coins_inserted;
    pthread_cond_t  coins_inserted_cond;
    pthread_mutex_t coins_inserted_lock;


    int64_t coins;
    pthread_mutex_t coins_lock;

    bool drink_dispensed;
    pthread_mutex_t drink_dispensed_lock;
    pthread_cond_t  drink_dispensed_cond;
    
} vending_machine_config;
    

/**
 * @brief Defaults to initialize vending_machine_config.
 */
enum VendingMachineConfigDefaults {
    Students       = 2,
    Capacity       = 4,
    CoinsInserted  = 0,
    Drinks         = 0,
    Coins          = 0,
    DrinkDispensed = false
};


/**
 * @brief Keeps track of student thread metadata.
 */
typedef struct student 
{
    int64_t id;
    vending_machine_config *config;
    pthread_t thread;
} student_t;


/**
 * @brief Runs student tasks.
 */
void* student(void*);


/**
 * @brief Runs machine tasks.
 */
void* machine(void*);


/**
 * @brief Runs supplier tasks.
 */
void* supplier(void*);


/**
 * @brief Initializes config.
 */
void config_init(int argc, char *argv[], vending_machine_config *config);


/**
 * @brief Spawns threads and waits for students to finish.
 */
void run(vending_machine_config* config);


/**
 * @brief Cleans up.
 */
void quit(vending_machine_config *config);


#endif // __VENDING_MACHINE_H
