#include <stdlib.h>

#include "vending_machine.h"


int main(int argc, char *argv[]) 
{
    vending_machine_config config;
    
    config_init(argc, argv, &config);
    run(&config);
    quit(&config);
    
    return EXIT_SUCCESS;
}
