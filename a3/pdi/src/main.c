#include <stdlib.h>

#include "pdi.h"

int main(int argc, char *argv[]) 
{
    pdi_config config;
    pdi_init(&config, argc, argv);
    pdi(&config);
    
    return EXIT_SUCCESS;
}
