#include <stdlib.h>

#include "prisoner.h"

int main(int argc, char *argv[]) 
{
    config prisoner_config;
    config_init(argc, argv, &prisoner_config);
    run(&prisoner_config);
    quit(&prisoner_config);
    
    return EXIT_SUCCESS;
}
